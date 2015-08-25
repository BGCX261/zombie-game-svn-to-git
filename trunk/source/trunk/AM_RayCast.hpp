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

#ifndef _AM_RayCast_hpp_
#define _AM_RayCast_hpp_

#include <NxOgreRaycaster.h>

#include "ComponentObject/AM_Component.hpp"

namespace Ogre
{
	class Vector3;
}

namespace NxOgre
{
	class Scene;
	class Actor;
}

namespace AutoMatic
{
	class RayCast
	{
	public:
		RayCast(NxOgre::Scene * scene_) 
			: _scene(scene_)
		{}

		bool cast(Ogre::Vector3 const & pos_, Ogre::Vector3 const & dir_, float maxDistance_);

		NxOgre::RayCastHit const & hit(void) const { return _hit; }

	private:
		NxOgre::Scene * _scene;

		NxOgre::RayCastHit _hit;
	};

	/// 
	/** Ray-cast from the camera through the mouse.
	*** Pass selection orders to an object if clicked on (and in the _selectableObjects).
	*/
	class MouseSelection : public RayCast
	{
	public:
		MouseSelection(NxOgre::Scene * scene_) 
			: RayCast(scene_), _selectedObject(0)
		{}

		/// Select an object at \a mouseX_ , \a mouseY_.
		/** If an object is not already selected, _selectedObject is set to the object hit.
		*** The _selectedObject then has the object hit passed to the select() method
			* of its order component (if available).
			* \param mouseX_ Percent across viewport of camera.
			* \param mouseY_ Percent up(?) viewport of camera.
			* \param maxDistance_ Furtherest point along the ray to cast.
		*/
		bool select(Ogre::Camera const & camera_, float mouseX_, float mouseY_, float maxDistance_);

		void selectedObject(Object * object_){ _selectedObject = object_; }
		Object * selectedObject(void){ return _selectedObject; }

		/// Add an object to the set that can be passed a select() order.
		bool add(Object * object_);
		/// Remove an object from the set that can be passed a select() order.
		bool remove(Object * object_);

	private:
		Object * _selectedObject;
		std::map<NxOgre::Actor *, Object *> _selectableObjects;
	};

//****************************************************************************//
}

#endif //_AM_RayCast_hpp_