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

#include <OgreVector3.h>
#include "ComponentObject/AM_Object.hpp"
#include "AM_Position.hpp"
#include "AM_Steering.hpp"

namespace AutoMatic
{
//****************************************************************************//
	
	Ogre::Vector3 ToOgreVector(OpenSteer::Vec3 const & vector_)
	{
		return Ogre::Vector3(vector_.x, vector_.y, vector_.z);
	}

	OpenSteer::Vec3 ToOpenSteerVector(Ogre::Vector3 const & vector_)
	{
		return OpenSteer::Vec3(vector_.x, vector_.y, vector_.z);
	}

//****************************************************************************//

	Steering::Steering(void) 
		: _hasTarget(false)
	{
		Component::Id(this);

		OpenSteer::SimpleVehicle::reset (); // reset the vehicle 

		setSpeed(1.5f);         // speed along Forward direction.
		setMaxForce(1000.0f);      // steering force is clipped to this magnitude
		setMaxSpeed(3.0f);         // velocity is clipped to this magnitude
	}

	// per frame simulation update
	void Steering::update(const float timestep_)
	{
	}

	void Steering::target(Ogre::Vector3 const & target_)
	{ 
		_target = ToOpenSteerVector(target_); 
		_hasTarget = true; 
	}

	void Steering::noTarget(void){ _hasTarget = false; }

	void Steering::onAttach(void)
	{
		if(Position * pos = owner()->component<Position>())
		{
			setPosition( ToOpenSteerVector(pos->position()) );
		}
	}

//****************************************************************************//

	SteeringManager::SteeringManager(void)
	{
	}

	SteeringManager::~SteeringManager(void) 
	{
	}

	void SteeringManager::update(const float timestep_)
	{
		for (std::set<Object *>::iterator i = _steeringObjects.begin(); i != _steeringObjects.end(); ++i)
		{
			(*i)->update<Steering>(timestep_);
		}
		
	}

//****************************************************************************//
}
