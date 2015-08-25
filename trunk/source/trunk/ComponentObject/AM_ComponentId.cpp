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

#include <cassert>
#include "AM_ComponentId.hpp"

namespace AutoMatic
{
	ComponentId::ComponentId(void)
	{
		_baseIds.insert(this);
	}

	ComponentId::~ComponentId(void)
	{
	}

	void ComponentId::inherit(ComponentId const * id_)
	{ 
		if(0 == id_){ return; }
		for(std::set<ComponentId const *>::const_iterator i = id_->_baseIds.begin(); 
			i != id_->_baseIds.end(); ++i)
		{
			_baseIds.insert((*i)); 
		}
	}
}
