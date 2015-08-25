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

#include "ComponentObject/AM_Object.hpp"

#include "AM_Position.hpp"
#include "AM_Physics.hpp"
#include "AM_Zombie.hpp"

namespace AutoMatic
{
//****************************************************************************//

	void ZombieSteering::update(float const timestep_)
	{
		if(_hasTarget)
		{
			applySteeringForce( steerForSeek(_target), timestep_);
		}
	}

//****************************************************************************//

	Object * ZombieFactory::create(NxOgre::Scene * scene_, NxOgre::Body * body_)
	{
		Object * zombie = new Object;
		zombie->component<Order>(new ZombieOrder);
		zombie->component<Steering>(new ZombieSteering);
		zombie->component<Position>(new Position);
		zombie->component<Physics>(new Physics(scene_, body_));

		return zombie;
	}

	void ZombieFactory::destroy(Object * zombie_)
	{
		delete zombie_;
	}
}
