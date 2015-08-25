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

#ifndef _AM_AI_Action_hpp_
#define _AM_AI_Action_hpp_

#include <vector>

#include "AM_AI_State.hpp"
#include "AM_AI_Brain.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		class Actor;

		/// The Action class should be overloaded to create different actions.
		/** Set up the pre and post states and overload the perform() method.
		* The Pre condition must be satisfied (<= current state) for the action
		* to be available.  The Post condition is the expected outcome of the action,
		* and determines success.  After an action is created and added to an AI,
		* it should not be changed.
		*/
		class Action
		{
		public:
			/// Each action may have many pre/post condition pairs.
			/** \todo Allow the AI to automatically fill out post conditions,
				* have more than one post for each pre.
			*/
			struct Condition
			{
				typedef std::string ID;

				/// Required pre-state to perform this action, in order to (possibly) lead to the post condition.
				/** A pre condition is satisfied if the _pre <= the current state.
				*/
				State & pre(void)
				{
					return _pre; 
				}

				/// Desired/expected post-state after the action is performed.
				/** Used to trace actions to goals.  It is not require for an action to have a post state, but it helps.
				*/
				State & post(void)
				{
					return _post; 
				}

				/// Does the \a state_ satisfy the _pre condition?
				/**
				***
				*/
				bool satisfiesPre(State const & state_) const;

				/// Does the \a condition_ _pre condition match this _pre condition?
				/**
				***
				*/
				bool satisfiesPre(Condition const & condition_) const;

			private:
				State _pre, _post;

				bool operator==(Condition const &) const;
			};

			enum Outcome { Succeeded, Completed, Failed, Impossible };

			typedef std::string ID;
			typedef boost::shared_ptr<Action> Ptr;

			Action(ID const & id_, Brain::Ptr const & brain_, bool isCritical_ = false) 
				: _id(id_), _brain(brain_), _currentState(0), _isCritical(isCritical_)
			{}

			virtual ~Action(void){}

			/// Perform this action by calling the action function.
			/** \param ai_ The AI calling this action. Useful if you want to set the
			* current state of the AI within this method.
			*/
			virtual Outcome perform(Actor & ai_)
			{
				return Impossible;
			}

			void currentState(State & owner_){ _currentState = &owner_; }

			State * currentState(void){ return _currentState; }

			ID const & id(void) const { return _id; }

			Condition & condition(Condition::ID const & id_)
			{
				return _conditions[id_];
			}

			std::map<Condition::ID, Condition> & conditions(void){ return _conditions; }

			/// Return a vector of condition ID's that the state_ satisfies.
			std::vector<Condition::ID> validConditions(State const & state_);

			/// Get the current value of the part for the current state (that the action was called from).
			Value::Ptr & isCurrentValue(Part::ID const & part_)
			{
				return currentState()->getValue(_brain->part(part_));
			}

			bool isCurrentValue(Part::ID const & part_, Value::ID const & value_)
			{
				Value::Ptr & value = currentState()->getValue(_brain->part(part_));
				if(value){ return value->id() == value_; }
				return false;
			}

			void pre(Condition::ID const & condition_, Part::ID const & part_, Value::ID const & value_)
			{
				_conditions[condition_].pre().setValue(_brain->part(part_), value_);
			}

			void post(Condition::ID const & condition_, Part::ID const & part_, Value::ID const & value_)
			{
				_conditions[condition_].post().setValue(_brain->part(part_), value_);
			}

			/// If a critical action is possible, then only critical actions will be possible, when performing an action in the state.
			bool isCritical(void) const { return _isCritical; }
			void isCritical(bool isCritical_){ _isCritical = isCritical_; }

		protected:
			bool operator==(Action const & action_) const;

			ID _id;
			Brain::Ptr _brain; /**< Pointer to the Brain that holds the _currentState **/
			State * _currentState; /**< State is a pointer because it is from the brain, and its values may change. **/
			std::map<Condition::ID, Condition> _conditions; /**< Conditions required for this action to be valid. **/

			bool _isCritical; /**< If a critical action can be performed, it takes precedence over any other non-critical actions. **/
		};

//****************************************************************************//
	}
}

#endif //_AM_AI_Action_hpp_