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

#ifndef BODY_H
#define BODY_H

#include <set>
#include "MathUtils.h"

namespace AutoMatic
{
	class Box2DQuadtree;
	class Box2DQuadtreeNode;
}

struct Body
{
	Body();
	void Set(const Vec2& w, float m);

	void AddForce(const Vec2& f)
	{
		force += f;
	}
	
	void GetBoudingNode(AutoMatic::Box2DQuadtree & quadtree_);

	void getBounds(void);
	std::set<Body *> * GetHit(void);

	Vec2 position;
	float rotation;

	Vec2 velocity;
	float angularVelocity;

	Vec2 biasedVelocity;
	float biasedAngularVelocity;

	Vec2 force;
	float torque;

	Vec2 width;

	float friction;
	float mass, invMass;
	float I, invI;

	AutoMatic::Box2DQuadtreeNode * _boundingNode;

	float _minX, _maxX, _minY, _maxY;
	Vec2 _v1, _v2, _v3, _v4;
};

#endif
