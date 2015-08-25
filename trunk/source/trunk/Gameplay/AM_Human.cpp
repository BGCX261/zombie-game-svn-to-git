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

#include "Serialisation/AM_Serialisation.hpp"
#include "ComponentObject/AM_Object.hpp"

#include "AM_Physics.hpp"
#include "AM_Position.hpp"
#include "AM_Steering.hpp"
#include "AM_Human.hpp"

namespace AutoMatic
{
	/// Order to human: \a object_ selected;
	void HumanOrder::select(Object * object_)
	{
		// Depending on context, do a number of things...
		if(owner() == object_)
		{ // This object has been selected.

		}
	}

	/// Order to human: \a point_ selected;
	void HumanOrder::point(Ogre::Vector3 const & point_)
	{
		if(Steering * s = owner()->component<Steering>())
		{
			s->target(point_);
		}
	}

	void HumanSteering::update(float const timestep_)
	{
		if(Position * pos = owner()->component<Position>())
		{
			setPosition(ToOpenSteerVector(pos->position()));
		}

		if(_hasTarget)
		{
			applySteeringForce(steerForSeek(_target), timestep_);
		}

		if(Position * pos = owner()->component<Position>())
		{
			pos->position(ToOgreVector(position()));
		}
	};

	Object * HumanFactory::create(NxOgre::Scene * scene_, NxOgre::Body * body_)
	{
		Object * human = new Object;
		human->component<Order>(new HumanOrder);
		human->component<Position>(new Position);
		human->component<Physics>(new Physics(scene_, body_));
		human->component<Steering>(new HumanSteering);
		human->component<Inventory>(new HumanInventory);
		
		return human;
	}

	void HumanFactory::destroy(Object * human_)
	{
		delete human_->component<Order>();
		delete human_->component<Steering>();
		delete human_->component<Inventory>();
		delete human_->component<Position>();
		delete human_->component<Physics>();
		delete human_;
	}

	void RunHumanSaveLoadTest(void)
	{
		HumanFactory humanFactory;
		Object * human = humanFactory.create(0,0);
		assert(human->component<Steering>());
		assert(human->component<HumanSteering>());
		assert(human->component<Order>());
		assert(human->component<HumanOrder>());
		assert(human->component<Inventory>());
		assert(human->component<HumanInventory>());

		{
			Serialise serialise("./tests/HumanSaveLoadTest.txt");
			serialise.out(human);
		}

		humanFactory.destroy(human);

		{
			Deserialise deserialise("./tests/HumanSaveLoadTest.txt");
			deserialise.in(human);
		}

		assert(human->component<Steering>());
		assert(human->component<HumanSteering>());
		assert(human->component<Order>());
		assert(human->component<HumanOrder>());
		assert(human->component<Inventory>());
		assert(human->component<HumanInventory>());

		humanFactory.destroy(human);
	}
}
