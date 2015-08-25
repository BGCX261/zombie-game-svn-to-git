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

#ifndef _AM_Order_hpp_
#define _AM_Order_hpp_

#include "ComponentObject/AM_Component.hpp"

namespace Ogre
{
	class Vector3;
}

namespace AutoMatic
{
	class Order : public Component
	{
	public:
		Order(void){ Component::Id(this); }

		/// Order: \a object_ selected;
		virtual void select(Object * object_) = 0;

		/// Order: \a point_ selected;
		virtual void point(Ogre::Vector3 const & point_) = 0;

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Component>(*this);
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Order, "AutoMaticOrder")

#endif //_AM_Order_hpp_
