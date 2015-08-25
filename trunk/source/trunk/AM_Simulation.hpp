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

#ifndef _AM_Simulation_hpp_
#define _AM_Simulation_hpp_

#include "AM_Physics.hpp"

namespace AutoMatic
{
	class Steering;
	class Simulation
	{
		public:
			enum State { Start_Up, Gui, Loading, Cancel_Loading, Update_World, User_Input, Render_World, Shutdown };

			Simulation(SteeringManager * steering_);
			virtual ~Simulation(void);
			
			State const & getCurrentState(void) const;
			bool requestStateChange(State const state_);
			
			virtual void update(float timestep_);

			World & world(void){ return _world; }

		protected:
			State _state;
			float minIdle, idleAccum, currDt;

			World _world;
			SteeringManager * _steering;

			virtual void updateWorld(float timestep_);
			virtual void updatePhysics(float timestep_);
			virtual void updateSteering(float timestep_);
	};
}

#endif //_AM_Simulation_hpp_
