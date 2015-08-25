/*
Zombie Project.  A game.  With Zombies.
Copyright (C) 2007  Matthew Chudleigh

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <list>
#include <vector>
#include <iostream>

#include "AM_Math.hpp"
#include "AM_Random.hpp"

#include "AM_AI_Action.hpp"
#include "AM_AI_State.hpp"

#include "AM_AI_Actor.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		Actor::Goal::Goal(void) 
			: _influence(0.0f) 
		{}

		Actor::Goal::Goal(State const & state_, float influence_) 
			: _state(state_), _influence(influence_) 
		{}

		Actor::Goal::Goal(Goal const & goal_) 
			: _state(goal_._state), _influence(goal_._influence) 
		{}

		bool Actor::Goal::operator==(Goal const & goal_) const
		{
			return goal_._state == _state;
		}

		bool Actor::Goal::operator<(Goal const & goal_) const
		{
			return goal_._state < _state;
		}

		Actor::Goal const & Actor::Goal::operator=(Actor::Goal const & goal_)
		{
			_state = goal_._state;
			_influence = goal_._influence;
			return *this;
		}

//****************************************************************************//
		struct ValidAction
		{
			Action::ID _actionID;
			Action::Condition::ID _condition;
			float _influence;

			ValidAction(void) : _actionID(""), _condition(""), _influence(0.0f) {}
			ValidAction(Action::ID const & actionID_, Action::Condition::ID const & condition_, float influence_)
				: _actionID(actionID_), _condition(condition_), _influence(influence_) {}

			bool operator==(ValidAction const & action_) const
			{
				return _actionID == action_._actionID && _condition == action_._condition;
			}

			bool operator<(ValidAction const & action_) const
			{
				return _actionID < action_._actionID ||
							 (_actionID == action_._actionID && _condition < action_._condition);
			}
		};

		std::set< ValidAction >::iterator 
			GetAction(Action::ID const & actionID_, Action::Condition::ID const & condition_,
			std::set< ValidAction > & validActions_)
		{
			for (std::set< ValidAction >::iterator i = 
						validActions_.begin(); i != validActions_.end(); ++i)
			{
				if( (*i)._actionID == actionID_ && (*i)._condition == condition_){ return i;}
			}
			return validActions_.end();
		}

		/// Find a path back from the goal to a valid action.
		/** If a path is found, add the goals influence to the influence of the action.
			* This does NOT determine the action taken - it merely adds influence to that action.
		*/
		void AddGoalInfluence( std::set< ValidAction > & validActions_, std::map<Action::ID, Action *> const & allActions_, 
			Actor::Goal goal_, std::list<std::pair<Action::ID, std::string> > & pathTaken_, float goalInfluence_)
		{
			for (std::map<Action::ID, Action *>::const_iterator j = allActions_.begin(); j != allActions_.end(); ++j)
			{ // For each available action...
				if(!(*j).second){ continue; }
				for (	std::map<std::string, Action::Condition>::iterator k = (*j).second->conditions().begin();
							k != (*j).second->conditions().end(); ++k)
				{ // For each of the conditions of the action...
					int partsFound = 0;
					if ( (partsFound = goal_.state().countOfPartsEqual((*k).second.post())) != 0 )
					{ // If the post condition satisfied the goal...
						std::set< ValidAction >::iterator foundValidAction;
						// Search for the Action/Condition on the ValidActions list...
						if ((foundValidAction = GetAction( (*j).first, (*k).first, validActions_ )) != validActions_.end())
						{ // The goalInfluence as carried (possibly) back from other actions.
							goalInfluence_ += (goal_.influence() * (static_cast<float>(partsFound) / static_cast<float>(goal_.state().countOfParts())));
							foundValidAction->_influence = goalInfluence_;
							goalInfluence_ = 0.0f;
							continue;
						}
						else
						{ // Found an action, that isn't (immediately) valid, but it does satisfy the goal...
							if(pathTaken_.end() != std::find(pathTaken_.begin(), pathTaken_.end(), 
								std::pair<Action::ID, std::string>((*j).first, (*k).first) ) )
							{ // Found the action already on the path, try another...
								// Prevents infinite loop... but is it the best way?
								continue;
							}
							else
							{ // Make the pre-condition the goal, and search for an action that satisfies the pre condition.
								pathTaken_.push_back(std::pair<Action::ID, std::string>((*j).first, (*k).first));
								AddGoalInfluence(validActions_, allActions_, 
									Actor::Goal((*k).second.pre(), goal_.influence()), pathTaken_, goalInfluence_);
								pathTaken_.pop_back();
							}
						}
					}
				}
			}
		}

		/// Wrapper, hides the need for the path list.
		void AddGoalInfluence( std::set< ValidAction > & validActions_, 
			std::map<Action::ID, Action *> const & allActions_, Actor::Goal goal_)
		{
			 std::list<std::pair<Action::ID, std::string> > pathTaken;
			AddGoalInfluence(validActions_, allActions_, goal_, pathTaken, 0.0f);
		}

		std::set<ValidAction> GetValidActions(State & state_, std::map<Action::ID, Action *> const & actions_)
		{
			std::set< ValidAction > validActions;
			for (std::map<Action::ID, Action *>::const_iterator j = actions_.begin(); j != actions_.end(); ++j)
			{
				if(!(*j).second){ continue; }

				for (	std::map<std::string, Action::Condition>::iterator k = (*j).second->conditions().begin();
					k != (*j).second->conditions().end(); ++k)
				{ // For each action
					if ( (*k).second.pre() <= state_ )
					{ // If the pre-condition is a subset of the current state...
						validActions.insert(ValidAction((*j).first, (*k).first, 0.0f));
					}
				}
			}

			return validActions;
		}

		Action * Actor::getAction(void)
		{
			Action * selectedAction = 0;
			if(!_currentState){ assert(false); return selectedAction; }

			// Find the current state in the set; add if it does not yet exist.
			State & actionState = _brain->getState(_currentState);
			
			// Find available actions by their pre condition being <= the current state.
			std::set< ValidAction > validActions = GetValidActions(actionState, _actions);

			// No actions available (no pre-conditions were satisfied by any action).
			if(validActions.size() == 0){ assert(false); return selectedAction; }

			for (std::set<Goal>::iterator i = _goals.begin(); i != _goals.end(); ++i)
			{
				AddGoalInfluence(validActions, _actions, *i);
			}

			// Get a set of valid actions, their influences and the total strength.
			std::map<Action::ID, float> validActionInfluences;
			int count = 0; bool hasUntriedActions = false, hasCriticalActions = false, isUntried = false;
			float min = 0.0f, totalOutcomeStrength = 0.0f;
			Action * action;
			for (std::set< ValidAction >::iterator j = validActions.begin(); j != validActions.end(); ++j)
			{
				action = _actions[(*j)._actionID];
				if( action && action->condition((*j)._condition).satisfiesPre(actionState) )
				{ // Isn't this if always going to be true, because of the test used to add a ValidAction in the first place?
					isUntried = false;
					std::pair<bool, float> const & outcome = actionState.outcome( (*j)._actionID );
					
					if(action->isCritical())
					{ // Critical actions take precedence over all others
						if(!hasCriticalActions)
						{
							validActionInfluences.clear();
							totalOutcomeStrength = 0.0f;
							min = 0.0f;
							count = 0;
						}
							
						hasCriticalActions = true;
					}
					else if(!outcome.first && !hasCriticalActions)
					{ // This is an untried action.  We want to try untried actions first.
						if(!hasUntriedActions)
						{ // Reset if necessary
							validActionInfluences.clear(); 
							totalOutcomeStrength = 0.0f;
							min = 0.0f;
							count = 0;
						}

						hasUntriedActions = true; isUntried = true; 
					}
					
					if( (!hasCriticalActions && !hasUntriedActions) ||	// IsNormal
							(hasUntriedActions && isUntried)						||	// IsUntried
							(hasCriticalActions && action->isCritical()))		// IsCritical
					{ // Allow tried actions unless there is one or more untried action.
						float temp = (*j)._influence + outcome.second;
						
						validActionInfluences[(*j)._actionID] += temp;
						totalOutcomeStrength += temp; 
						
						if(temp < min){ min = temp; }
					}

					count++;
				}
				else
				{
					assert(false);
				}
			}

			// The reason for this is to allow safe negative strengths.
				// Min will be <= 0.  
			totalOutcomeStrength += -1.0f * min * count;
			assert(totalOutcomeStrength >= 0.0f);
			float ran = (totalOutcomeStrength <= 0.0f ? 0.0f : Random::Get()->randomF(0.0f, totalOutcomeStrength));
			float debugTotalOut = totalOutcomeStrength;
			
			// Reset the value, just so I can use it again.
			totalOutcomeStrength = 0.0f;
			// Randomly select action.
			for (std::map<Action::ID, float>::iterator j = validActionInfluences.begin(); j != validActionInfluences.end(); ++j)
			{ // For each valid action, add the (shifted) influence
				totalOutcomeStrength += (*j).second + min;
				if(totalOutcomeStrength >= ran)
				{ // The cumulative total exceeds the random number, use this action.
					selectedAction = _actions[(*j).first]; break;
				}
			}

			// Failure - TODO: this could maybe happen due to floating point error?
			if(!selectedAction){ assert(false); return selectedAction; }

			assert(_currentState.countOfOutcomes() == 0);
			_currentState.clear();
			// Perform the action.
			selectedAction->currentState(actionState);
			return selectedAction;
		}
	
		std::set<Actor::Goal> Actor::goalsSatisfied(void)
		{
			std::set<Actor::Goal> satisfiedGoals;
			for (std::set<Actor::Goal>::iterator i = _goals.begin(); i != _goals.end(); ++i)
			{
				if( (*i).state() <= _currentState )
				{
					satisfiedGoals.insert(*i);
				}
			}
			return satisfiedGoals;
		}
	}	
}
