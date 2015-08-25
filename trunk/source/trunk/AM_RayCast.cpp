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
#include <NxOgreBody.h>

#include "ComponentObject/AM_Object.hpp"
#include "Gameplay/AM_Order.hpp"
#include "AM_Physics.hpp"
#include "AM_RayCast.hpp"

namespace AutoMatic
{
	bool RayCast::cast(Ogre::Vector3 const & pos_, Ogre::Vector3 const & dir_, float maxDistance_)
	{
		NxOgre::RayCaster rayCast(pos_, dir_, maxDistance_, NxOgre::RayCaster::RCT_CLOSEST, _scene);
		if(rayCast.castShape(NxOgre::RayCaster::AF_NONE))
		{
			_hit = rayCast.getClosestRaycastHit();
			return true;	
		}
		else
		{
			return false;
		}
	}

//****************************************************************************//

	bool MouseSelection::select(Ogre::Camera const & camera_, float mouseX_, float mouseY_, float maxDistance_)
	{
		Ogre::Ray ray = camera_.getCameraToViewportRay(mouseX_, mouseY_);
		if(cast(ray.getOrigin(), ray.getDirection(), maxDistance_))
		{
			Order * order = 0;
			NxOgre::Actor * actor = hit().mActor;
			Object * & selected = _selectableObjects[hit().mActor];
			if(!_selectedObject){ _selectedObject = selected; }
			if(_selectedObject && (order = _selectedObject->component<Order>()) )
			{ 
				if(selected)
				{
					order->select(selected); 
				}
				else
				{
					order->point(hit().mWorldImpact);
				}
			}
			return true;
		}

		return false;
	}

	bool MouseSelection::add(Object * object_)
	{ 
		if(Physics * physics = object_->component<Physics>())
		{
			if(!physics->body()){ return false; }
			_selectableObjects[physics->body()] = object_;
			return true;
		}
		return false; 
	}

	bool MouseSelection::remove(Object * object_)
	{ 
		if(Physics * physics = object_->component<Physics>())
		{
			if(!physics->body()){ return false; }
			assert(0 == _selectableObjects[physics->body()] || object_ == _selectableObjects[physics->body()]);
			_selectableObjects[physics->body()] = 0;
			return true;
		}
		return false; 
	}
}
