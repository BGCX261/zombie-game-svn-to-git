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

#include <NxOgre.h>

#include "AM_Steering.hpp"
#include "AM_Simulation.hpp"

namespace AutoMatic
{
	Simulation::Simulation(SteeringManager * steering_) 
		: _state(Start_Up), minIdle(0.03f), idleAccum(0.0f), currDt(0.0f)
		, _steering(steering_)
	{
	}

	Simulation::~Simulation(void)
	{
	}


	Simulation::State const & Simulation::getCurrentState(void) const
	{
		return _state;
	}

	bool Simulation::requestStateChange(State const state_) 
	{
		// Shutdown state cannot be changed once initiated
		if(Shutdown == _state){	return false;	}
		_state = state_;
		return true;
	}

	void Simulation::update(float timestep_)
	{
		switch(_state)
		{
		case Update_World:
			{
				updateWorld(timestep_);
			}
			break;
		}
	}

	void Simulation::updateWorld(float timestep_)
	{
		idleAccum += timestep_;

		if(idleAccum >= minIdle) // TODO: maxIdle?
		{
			currDt = timestep_;
			while(idleAccum > 0.0)
			{
				idleAccum -= currDt;

				if(idleAccum < 0.0)
				{ 
					currDt += idleAccum;
					idleAccum = 0.0; 
				}

				updatePhysics(timestep_);
				updateSteering(timestep_);
			}
		}
	}

	void Simulation::updatePhysics(float timestep_)
	{
		_world.update(timestep_);
	}

	void Simulation::updateSteering(float timestep_)
	{
		_steering->update(currDt);
	}

}
