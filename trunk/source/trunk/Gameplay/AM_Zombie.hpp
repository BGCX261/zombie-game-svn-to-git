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

#ifndef _AM_Zombie_hpp_
#define _AM_Zombie_hpp_

#include "AM_Order.hpp"
#include "AM_Steering.hpp"

namespace NxOgre
{
	class Scene;
	class Body;
}

namespace AutoMatic
{
	class ZombieOrder : public Order
	{
	public:
		ZombieOrder(void){ Component::Id(this); }

		/// Order to zombie: \a object_ selected;
		virtual void select(Object * object_){}

		/// Order to zombie: \a point_ selected;
		virtual void point(Ogre::Vector3 const & point_){}

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Order>(*this);
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::ZombieOrder, "AutoMaticZombieOrder")

namespace AutoMatic
{
	class ZombieSteering : public Steering
	{
	public:
		ZombieSteering(void){ Component::Id(this); }
		virtual void update(float const timestep_);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Steering>(*this);
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::ZombieSteering, "AutoMaticZombieSteering")

namespace AutoMatic
{
	class ZombieFactory
	{
	public:
		Object * create(NxOgre::Scene * scene_, NxOgre::Body * body_);
		void destroy(Object * zombie_);
	};
}

#endif // _AM_Zombie_hpp_
