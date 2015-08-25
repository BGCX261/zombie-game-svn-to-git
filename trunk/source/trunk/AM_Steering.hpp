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

#ifndef _AM_Steering_hpp_
#define _AM_Steering_hpp_

#include <OgreVector3.h>

#include "Serialisation/AM_SerialisationIncludes.hpp"

#include "openSteer/SimpleVehicle.h"
#include "openSteer/Obstacle.h"

#include "ComponentObject/AM_Component.hpp"

namespace AutoMatic
{
	/// Convert an OpenSteer:Vec3 to an Ogre::Vector3.
	Ogre::Vector3 ToOgreVector(OpenSteer::Vec3 const & vector_);
	/// Convert an Ogre::Vector3 to an OpenSteer::Vec3.
	OpenSteer::Vec3 ToOpenSteerVector(Ogre::Vector3 const & vector_);

	/// 
	/** General steering component, stores a single target point.
	***
	*/
	class Steering : public OpenSteer::SimpleVehicle, public Component
	{
	public:
		
		Steering (void);

		// Per-frame simulation update
		virtual void update(float const timestep_);

		/// Set the Steering target.
		void target(Ogre::Vector3 const & target_);
		/// Remove the Steering target.
		void noTarget(void);

		virtual void onAttach(void);

	protected:
		bool _hasTarget; /**< Is _target set? **/
		OpenSteer::Vec3 _target; /**< Target point of the steering. **/

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Component>(*this);
			ar_ & _hasTarget;
			ar_ & _target.x; ar_ & _target.y; ar_ & _target.z;
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Steering, "AutoMaticSteering")

namespace AutoMatic
{
	class Obstacle
	{
	public:
		Obstacle(void){}

	private:	
		OpenSteer::Obstacle * _obstacle;
	};

	/// 
	/** Organise and update objects with Steering capability.
	*** \todo Ensure the Steering component is available?
	*/
	class SteeringManager
	{
	public:
		SteeringManager(void);

		virtual ~SteeringManager(void);

		void update(const float timestep_);

		void add(Object * object_){ _steeringObjects.insert(object_); }
		void remove(Object * object_){ _steeringObjects.erase(object_); }

	private:
		std::set<Object *> _steeringObjects;
	};
}

#endif // _AM_Steering_hpp_