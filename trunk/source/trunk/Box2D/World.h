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

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <set>

#include "Box2DQuadtree.hpp"

#include "MathUtils.h"
#include "Arbiter.h"

struct Body;
struct Joint;

struct World
{
	World(Vec2 gravity, int iterations, int worldSize_, int minWidth_) 
		: gravity(gravity), iterations(iterations)
		, _quadtree(worldSize_, minWidth_)
	{}

	void Add(Body* body);
	void Add(Joint* joint);
	void Clear();

	void Step(float dt, bool bruteForce_);

	void BroadPhase(bool bruteForce_);

	std::vector<Body*> bodies;
	std::vector<Joint*> joints;
	std::set<Arbiter> arbiters;
	Vec2 gravity;
	int iterations;

	int _collisionChecks, _contacts, _numUpdates;
	AutoMatic::Box2DQuadtree _quadtree;

	void arbitrateBodies(Body * bi_, Body * bj_);
	void recursiveCollisionCheck(AutoMatic::Box2DQuadtreeNode * node_);
};

#endif
