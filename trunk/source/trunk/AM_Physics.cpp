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

#include <cassert>
#include <NxOgre.h>

#include "ComponentObject/AM_Object.hpp"
#include "AM_Position.hpp"
#include "AM_Physics.hpp"

namespace AutoMatic
{
	Physics::Physics(NxOgre::Scene * scene_, NxOgre::Body * body_)
		: _scene(scene_), _body(body_)
	{
		Component::Id(this);
	}

	Physics::~Physics(void)
	{
		if(_scene){ _scene->destroyBody(_body->getName()); }
	}

	void Physics::update(float const timestep_)
	{
		if(!_body){ return; }

		if(Position * pos = owner()->component<Position>())
		{
			if(timestep_ <= 0.0f)
			{ // TODO: This is such a HACK to get a pre/post system for the physics (like steering).
					// TODO: Are there user callbacks for this in the NxOgre system??
				_body->setGlobalPosition(pos->position());
			}
			else
			{
				pos->position(_body->getGlobalPosition());
			}
		}
	}

	void Physics::onAttach(void)
	{
		if(!_body){ return; }
		if(Position * pos = owner()->component<Position>())
		{
			pos->position(_body->getGlobalPosition());
		}
	}

	void Physics::onEnable(void)
	{
		if(_scene)
		{
		}
	}

	void Physics::onDisable(void)
	{
		if(_scene)
		{

		}
	}

//****************************************************************************//

	World::World(void)
	{
		_world = new NxOgre::World;
	}

	World::~World(void)
	{
		delete _world;
	}

	NxOgre::World * World::world(void)
	{
		return _world;
	}

	void World::update(float timestep_)
	{
		for (std::set<Object *>::iterator i = _physialObjects.begin(); i != _physialObjects.end(); ++i)
		{
			(*i)->update<Physics>(-1.0f); // TODO: This is SUCK A HACK.  See Physics::update (above).
		}
		_world->simulate(timestep_);
		for (std::set<Object *>::iterator i = _physialObjects.begin(); i != _physialObjects.end(); ++i)
		{
			(*i)->update<Physics>(timestep_);
		}
	}
}
