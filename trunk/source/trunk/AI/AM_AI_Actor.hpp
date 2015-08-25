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

#ifndef _AM_AI_Actor_hpp_
#define _AM_AI_Actor_hpp_

#include <boost/shared_ptr.hpp>
#include <string>
#include <set>
#include <map>

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		/// The Actor has a state, a number of actions and a number of goals.
		/** \todo Actions should not be allowed to changed after they are added to the map.
			* This is because a state that could once call an action may no longer be able to.
		*/
		class Actor
		{
		public:
			class Goal
			{
			public:
				typedef	boost::shared_ptr<Goal> Ptr;

				Goal(void);

				Goal(State const & state_, float influence_);

				Goal(Goal const & goal_);
	
				bool operator==(Goal const & goal_) const;

				bool operator<(Goal const & goal_) const;

				Goal const & operator=(Goal const & goal_);

				State & state(void) { return _state; }
				float influence(void) const { return _influence; }

				void influence(float influence_) { _influence = influence_; }

			private:
				State _state;
				float _influence;
			};

			Actor(Brain::Ptr const & brain_) : _brain(brain_) {}
			
			void addAction(Action * action_)
			{
				_actions[action_->id()] = action_;
			}

			void remAction(Action::ID const & action_)
			{
				_actions[action_] = 0;
			}

			/// Returns an action pointer reference based on the \a id_.
			Action * action(Action::ID const & id_)
			{
				return _actions[id_]; 
			}

			/// Return the current state.
			/** Set the values of the AI's current state before calling getAction(),
				* and after completing the action.
			*/
			State & currentState(void)
			{ 
				return _currentState;
			}

			std::set<Goal> & goals(void) { return _goals; }

			void addGoal(Goal const & goal_){ _goals.insert(goal_); }
			void remGoal(Goal const & goal_){ _goals.erase(goal_); }

			void currentValue(Part::ID const & part_, Value::ID const & value_)
			{
				currentState().setValue( _brain->part(part_), value_);
			}

			/// Obtain a set of goals satisfied by the current state.
			std::set<Actor::Goal> goalsSatisfied(void);

			/// Finds an action and resets the current state if successful.
			/** If an action is found, the currentState is reset.  It should have its 
				* values reentered after the action, so it can be used to check the post
				* condition of the action.
				* \return The action to be performed if successful, null if failed.
			*/
			Action * getAction(void);

		private:
			Brain::Ptr _brain;

			std::map<Action::ID, Action *> _actions;
			std::set<Actor::Goal> _goals;

			State _currentState;
		};

//****************************************************************************//
	}
}

/*
Scenario: Behind cover, under attack.

The character has needs: 
1. To stay alive.  
2. To defeat the enemy.

and ways of fulfilling those needs.
1. Hide in cover.
2. Fire at the enemy.

Hiding behind cover for the fight will not defeat the enemy, 
so the AI has to take risks by leaving cover to fire upon the enemy.  
Staying out of cover all the time will not keep the AI alive, 
so the AI has to take cover when the risk of being killed becomes too great. 

The current state of the AI influences these leave/take cover decisions.  
If the weapon is out of ammunition, then leaving cover would be a bad decision 
because there is no way to attack the enemy.  

If the AI is injured, then they are less likely to try to attack; 
maybe if they are mortally wounded, they would relentlessly attack the enemy 
(the need to stay alive is failed, but not yet the need to defeat the enemy, 
so there is only one course of action to take).  As the AI holds cover, 
the desire to attack the enemy rises, while it has the means (ammo).  
When the AI leaves cover, the desire to stay alive rises until it exceeds the 
desire to kill the enemy.  Because returing to cover is a means of staying alive, 
the AI does that.  

If a grenade is thrown into the AI's cover, it should bolt out.  
The desire for cover still exists, but the area it is in is no longer cover.

//******************************************************************************

A state is defined as a unique set of values, of a specific size.  States are not
equal if one is a subset of the other, as this would mean that the smaller state
has some unknown values for those in the larger state.

Within a state certain actions are allowed.  Actions are defined as a set of pre
condition values, and a set of post condition desired/expected values.  Pre condition
values must be met for the action to be allowed and post condition values are what
is expected when the action succeeds.  Successes are recorded against total attempts
of an action in a particular state, i.e., the state that tries an action records 
successes and failures.  This percentage influences the probability of the action
being performed again when the state is next encountered.

AI's have goals, defined as a set of desired values.  Using expected action outcomes,
the AI can find a path from the current state to those values.  Goals influence
what action is taken.
*/

#endif //_AM_AI_Actor_hpp_
