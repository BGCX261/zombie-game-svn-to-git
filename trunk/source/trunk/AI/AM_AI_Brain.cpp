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

#include "AM_AI_Brain.hpp"

namespace AutoMatic
{
	namespace AI
	{
		Part::Ptr & Brain::part(Part::ID const & id_)
		{
			Part::Ptr & ret = _parts[id_];
			if(!ret){ ret.reset(generatePart(id_)); }
			return ret;
		}

		State & Brain::getState(State & current_)
		{
			State::Set::iterator actionStateItr = std::find(_stateHistory.begin(), _stateHistory.end(), current_);

			if(actionStateItr == _stateHistory.end())
			{ 
				_stateHistory.insert(current_);  
				actionStateItr = std::find(_stateHistory.begin(), _stateHistory.end(), current_);
			}

			assert(actionStateItr != _stateHistory.end());

			return (*actionStateItr);
		}

		Part * Brain::generatePart(Part::ID const & id_)
		{
			return new Part(id_);
		}
	}
}