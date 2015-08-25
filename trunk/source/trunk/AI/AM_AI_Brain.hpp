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

#ifndef _AM_AI_Brain_hpp_
#define _AM_AI_Brain_hpp_

#include <map>
#include <boost/shared_ptr.hpp>

#include "AM_AI_Part.hpp"
#include "AM_AI_State.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		/// The Brain is shared between Actor's.
		/**
		***
		*/
		class Brain
		{
		public:
			typedef boost::shared_ptr<Brain> Ptr;
			
			virtual ~Brain(void){}

			/// Retrieve a part that matches \a id_, or create a new one.
			/**
			***
			*/
			Part::Ptr & part(Part::ID const & id_);

			/// Find a State that matches \a current_, or create a new one.
			/** The \a current_ state is considered new if it is not equal to 
			*** any of the other states in the _stateHistory.  I.e., no other state
				* has the same Part/Value pair, for each Part/Value pair in the
				* \a current_ state.
				* \sa State::operator==
			*/
			State & getState(State & current_);

		protected:

			State::Set _stateHistory; /**< Past states, and the action outcomes. **/
			std::map<Part::ID, Part::Ptr> _parts; /**< Parts available to generate a state. **/

			/// Create a new Part object.
			/** 
			***
			*/
			virtual Part * generatePart(Part::ID const & id_);
		};

//****************************************************************************//
	}
}

#endif //_AM_AI_Brain_hpp_