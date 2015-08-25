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

#ifndef _AM_AI_State_hpp_
#define _AM_AI_State_hpp_

#include <set>

#include "AM_AI_Part.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		/// Holds a state for the AI, goal or action.
		/** Add as many Part/Value pairs as needed to describe the state.
			* Action pre/post states and goal states will be compared <= the current 
			* state.
		*/
		class State
		{
		public:
			typedef std::set<State> Set;

			State(void){}

			/// Assign \a copy_'s values to this state.
			void setValues(State const & copy_)
			{
				_parts = copy_._parts;
			}

			/// Completely clear out the _values map.
			void clear(void);

			/// Sets the values for the \a part_ to the \a valueID_.
			/** Does not allow null \a part_ or bad \a valueID_.
			*/
			void setValue(Part::Ptr & part_, Value::ID const & valueID_);

			/// Get the value held in _values for the \a logic_;
			/** \todo Don't allow null values to be added to the set...
			*/
			Value::Ptr & getValue(Part::Ptr & part_)
			{
				return _parts[part_]; 
			}

			/// Returns true if the State's are equal.
			/** States must be the same size (i.e., have the same number of Logic::Ptr's).
				* \sa isEqual()
			*/
			bool operator==(State const & state_) const;

			/// Returns true if this State is equal to the \a state_.
			/** Ignores size if this state has fewer members than \a state_.  
				* Returns false if this state is larger than \a state_.
				* \sa isEqual()
			*/
			bool operator<=(State const & state_) const;

			/// This is just for sorting in the set.  Only compares pointers.
			bool operator<(State const & state_) const;

			/// Returns true if the size of _values is > 0 and every value is valid (i.e., not null).
			operator bool(void) const;

			/// Retrieve an outcome for an action, by ID.  
			/** \returns A bool/float pair.  The first is true if the action has been tried in this state.
			* The second is the outcome for the action in this state.
			*/
			std::pair<bool, float> const & outcome(std::string const & action_) { return _actionOutcomes[action_]; }

			/// Set an actions outcome in this state.
			void outcome(std::string const & action_, float outcome_)
			{
				std::pair<bool, float> & p = _actionOutcomes[action_];
				p.first = true; p.second = outcome_;
			}

			void addToOutcome(std::string const & action_, float outcome_)
			{
				std::pair<bool, float> & p = _actionOutcomes[action_];
				p.first = true; p.second += outcome_;
			}

			/// How many parts of this state does \a state_ satisfy?
			int countOfPartsEqual(State const & state_) const;

			int countOfParts(void) const { return _parts.size(); }

			int countOfOutcomes(void){ return _actionOutcomes.size(); }

		protected:
			/// Returns true if all Logic/Set pairs in the \a smaller_ state are in the \a bigger_ state.
			/** Both states must have the same Logic::Ptr's and have the same Set::Ptr's for each Logic::Ptr.
				* NOTE: They are only comparing pointers!  Rationale: The Logic objects control
				* the creation of Set objects, and Set objects are entirely owned by the Logic object
				* that created them, so it doesn't make sense to have a Logic/Set pair with a Set
				* object not created by the Logic object.
			*/
			bool isEqual(State const & smaller_, State const & bigger_) const;

			std::map< Part::Ptr, Value::Ptr > _parts; /**< Part/Value pairs of this State. **/

			std::map< std::string, std::pair<bool, float> > _actionOutcomes; /**< [Action::ID / [Has been performed / Outcome]] **/

			//Desirability of state - How bad does the AI want to enter this state?
		};

//****************************************************************************//
	}
}

#endif //_AM_AI_State_hpp_
