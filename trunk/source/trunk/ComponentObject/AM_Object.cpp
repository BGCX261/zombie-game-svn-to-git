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
#include "AM_Component.hpp"
#include "AM_Object.hpp"

namespace AutoMatic
{
	Object::~Object(void)
	{
		for (std::map<ComponentId const *, Component *>::iterator i = _components.begin();
				 i != _components.end(); ++i)
		{
			if((*i).second)
			{ 
				(*i).second->onRemove();
				(*i).second->owner(0); 
			}
		}
	}

	void Object::remove(Component * component_)
	{
		if(component_)
		{
			for (ComponentId::Iter i = component_->classId()->begin(); 
					 i != component_->classId()->end(); ++i)
			{
				Component * & c = _components[(*i)];
				if(c)
				{
					assert(0 == component_->owner() || this == component_->owner());
					component_->onRemove();
					component_->owner(0);
					c = 0;
				}
			}			
		}
	}
}
