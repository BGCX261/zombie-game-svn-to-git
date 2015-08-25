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

#ifndef _AM_Position_hpp_
#define _AM_Position_hpp_

#include <OgreVector3.h>

#include "ComponentObject/AM_Component.hpp"

namespace AutoMatic
{
	/// 
	/** The position component will be used to get/set the general object position.
	*** E.g., the physics will set the object position, and the steering will get the 
		* object position (not the physical position).  It is a general storage for separate systems.
	*/
	class Position : public Component
	{
	public:
		Position(void)
		{
			Component::Id(this);
		}

		void position(Ogre::Vector3 const & position_){ _position = position_; }
		Ogre::Vector3 const & position(void) const { return _position; }

	private:
		Ogre::Vector3 _position;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Component>(*this);
			ar_ & _position.x; ar_ & _position.y; ar_ & _position.z;
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Position, "AutoMaticPosition")


#endif // _AM_Position_hpp_