/*
* Copyright (c) 2006 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#include "World.h"
#include "Body.h"
#include "Joint.h"

#include <windows.h>
#include <gl\gl.h>

using std::vector;
using std::set;

typedef set<Arbiter>::iterator ArbIter;

void World::Add(Body* body)
{
	bodies.push_back(body);
}

void World::Add(Joint* joint)
{
	joints.push_back(joint);
}

void World::Clear()
{
	_quadtree.clear();
	bodies.clear();
	joints.clear();
	arbiters.clear();
}

void World::arbitrateBodies(Body * bi_, Body * bj_)
{
	if (bi_->invMass == 0.0f && bj_->invMass == 0.0f)
	{
		return;
	}

	Arbiter newArb(bi_, bj_);

	if (newArb._numContacts > 0)
	{
		_contacts += newArb._numContacts;
		ArbIter arb = arbiters.find(newArb);
		if (arb == arbiters.end())
		{
		arbiters.insert(newArb);
		}
		else
		{
			_numUpdates += 1;
			(*arb).Update(newArb._contacts, newArb._numContacts);
		}
	}
	else
	{
		arbiters.erase(newArb);
	}
}

void World::recursiveCollisionCheck(AutoMatic::Box2DQuadtreeNode * node_)
{
	std::set<Body *>::iterator i = node_->bodies().begin();

	while (i !=  node_->bodies().end())
	{ // For each of the nodes in the set,
		if( (*i)->_boundingNode == node_ )
		{ // If the node belongs to this level,
			std::set<Body *>::iterator j;
			for (j = node_->bodies().begin();
				j !=  node_->bodies().end(); ++j)
			{ // Check against every other body still in the set, except itself.
				if( *i != *j )
				{ 
					_collisionChecks++;
					arbitrateBodies((*i), (*j)); 
				}
			}

			j = i; i++;
			node_->bodies().erase(j); // Remove the tested body.
		}
		else
		{ // Keep in the set, for other nodes on the level to check.
			i++;
		}
	}

	node_->bodies().clear();

	AutoMatic::QuadtreeNode ** c = node_->children(false);
	if(c)
	{
		for (int i = 0; i < 4; ++i)
		{
			AutoMatic::Box2DQuadtreeNode * t = static_cast<AutoMatic::Box2DQuadtreeNode *>(c[i]);
			if(t->bodies().size() > 0)
			{
				recursiveCollisionCheck(t);
			}
		}
	}
}

void World::BroadPhase(bool bruteForce_)
{
	if(bruteForce_)
	{
		for (int i = 0; i < (int)bodies.size(); ++i)
		{
			Body* bi = bodies[i];

			for (int j = i + 1; j < (int)bodies.size(); ++j)
			{
				_collisionChecks++;
				arbitrateBodies(bi, bodies[j]);
			}
		}
	}
	else
	{
		recursiveCollisionCheck(static_cast<AutoMatic::Box2DQuadtreeNode *>(_quadtree.root()));
		
		std::set<Arbiter>::iterator i = arbiters.begin();
		while (i != arbiters.end())
		{
			if( !(*i)._wasUpdated )
			{
				std::set<Arbiter>::iterator j = i; ++i;
				arbiters.erase(j);
			}
			else
			{
				(*i)._wasUpdated = false;
				++i;
			}
		}
	}
}

void World::Step(float dt, bool bruteForce_)
{
	float inv_dt = dt > 0.0f ? 1.0f / dt : 0.0f;

	BroadPhase(bruteForce_);
	
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* b = bodies[i];

		if (b->invMass == 0.0f)
			continue;

		b->velocity += dt * (gravity + b->invMass * b->force);
		b->angularVelocity += dt * b->invI * b->torque;
	}

	for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
	{
		(*arb).PreStep(inv_dt);
	}

	for (int i = 0; i < (int)joints.size(); ++i)
	{
		joints[i]->PreStep(inv_dt);	
	}

	for (int i = 0; i < iterations; ++i)
	{
		for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
		{
			(*arb).ApplyImpulse();
		}

		for (int j = 0; j < (int)joints.size(); ++j)
		{
			joints[j]->ApplyImpulse();
		}
	}

	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* b = bodies[i];

		b->position += dt * (b->velocity + b->biasedVelocity);
		b->rotation += dt * (b->angularVelocity + b->biasedAngularVelocity);

		// Bias velocities are reset to zero each step.
		b->biasedVelocity.Set(0.0f, 0.0f);
		b->biasedAngularVelocity = 0.0f;

		b->force.Set(0.0f, 0.0f);
		b->torque = 0.0f;

		b->getBounds();

		if(!bruteForce_){ b->GetBoudingNode(_quadtree); }
	}
}
