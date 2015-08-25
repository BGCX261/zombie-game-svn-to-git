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

#include "AM_AI_Part.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		Value::Ptr & Part::getValue(Value::ID const & id_)
		{
			Value::Ptr & ret = _values[id_];
			if(!ret){ ret.reset(generateValue(id_)); }
			return ret;
		}

//****************************************************************************//
	}
}
