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

#ifndef _AM_Physics_hpp_
#define _AM_Physics_hpp_

#include <set>
#include "ComponentObject/AM_Component.hpp"

namespace NxOgre
{
	class Body;
	class Scene;
	class World;
}

namespace AutoMatic
{
	class Physics : public Component
	{
	public:
		Physics(NxOgre::Scene * scene_ = 0, NxOgre::Body * body_ = 0);

		virtual ~Physics(void);

		virtual void update(float const timestep_);
	
		void body(NxOgre::Body * body_){ _body = body_; }
		void scene(NxOgre::Scene * scene_){ _scene = scene_; }

		NxOgre::Body * body(void){ return _body; }

	protected:

		virtual void onAttach(void);
		virtual void onEnable(void);
		virtual void onDisable(void);	

	private:
		NxOgre::Body * _body;
		NxOgre::Scene * _scene;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Component>(*this);
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Physics, "AutoMaticPhysical")

namespace AutoMatic
{
	class World
	{
	public:
		World(void);
		~World(void);

		NxOgre::World * world(void);

		void update(float timestep_);

		void add(Object * object_){ _physialObjects.insert(object_); }
		void remove(Object * object_){ _physialObjects.erase(object_); }

	private:
		NxOgre::World * _world;
		std::set<Object *> _physialObjects;
	};
}

#endif // _AM_Physics_hpp_