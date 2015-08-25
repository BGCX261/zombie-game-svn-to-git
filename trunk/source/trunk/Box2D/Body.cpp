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

#include "Box2DQuadtree.hpp"

#include "Body.h"

Body::Body()
	: _boundingNode(0)
{
	position.Set(0.0f, 0.0f);
	rotation = 0.0f;
	velocity.Set(0.0f, 0.0f);
	angularVelocity = 0.0f;
	biasedVelocity.Set(0.0f, 0.0f);
	biasedAngularVelocity = 0.0f;
	force.Set(0.0f, 0.0f);
	torque = 0.0f;
	friction = 0.2f;

	width.Set(1.0f, 1.0f);
	mass = FLT_MAX;
	invMass = 0.0f;
	I = FLT_MAX;
	invI = 0.0f;
}

void Body::Set(const Vec2& w, float m)
{
	position.Set(0.0f, 0.0f);
	rotation = 0.0f;
	velocity.Set(0.0f, 0.0f);
	angularVelocity = 0.0f;
	biasedVelocity.Set(0.0f, 0.0f);
	biasedAngularVelocity = 0.0f;
	force.Set(0.0f, 0.0f);
	torque = 0.0f;
	friction = 0.2f;

	width = w;
	mass = m;

	if (mass < FLT_MAX)
	{
		invMass = 1.0f / mass;
		I = mass * (width.x * width.x + width.y * width.y) / 12.0f;
		invI = 1.0f / I;
	}
	else
	{
		invMass = 0.0f;
		I = FLT_MAX;
		invI = 0.0f;
	}
}

void Body::GetBoudingNode(AutoMatic::Box2DQuadtree & quadtree_)
{
	if(!_boundingNode) { _boundingNode = static_cast<AutoMatic::Box2DQuadtreeNode *>(quadtree_.root()); }

	_boundingNode = static_cast<AutoMatic::Box2DQuadtreeNode *>(_boundingNode->container(_minX, _minY, _maxX, _maxY));
	assert(_boundingNode);

	_boundingNode->add(this);
}

void Body::getBounds(void)
{
	Mat22 const R(rotation);
	Vec2 const h = 0.5f * width;

	_v1 = position + R * Vec2(-h.x, -h.y);
	_minX = _maxX = _v1.x; _minY = _maxY = _v1.y;

	_v2 = position + R * Vec2( h.x, -h.y);
	if(_minX > _v2.x){ _minX = _v2.x; }
	else if(_maxX < _v2.x){ _maxX = _v2.x; }
	if(_minY > _v2.y){ _minY = _v2.y; }
	else if(_maxY < _v2.y){ _maxY = _v2.y; }

	_v3 = position + R * Vec2( h.x,  h.y);
	if(_minX > _v3.x){ _minX = _v3.x; }
	else if(_maxX < _v3.x){ _maxX = _v3.x; }
	if(_minY > _v3.y){ _minY = _v3.y; }
	else if(_maxY < _v3.y){ _maxY = _v3.y; }

	_v4 = position + R * Vec2(-h.x,  h.y);
	if(_minX > _v4.x){ _minX = _v4.x; }
	else if(_maxX < _v4.x){ _maxX = _v4.x; }
	if(_minY > _v4.y){ _minY = _v4.y; }
	else if(_maxY < _v4.y){ _maxY = _v4.y; }

	_minX *= 10.0f; _maxX *= 10.0f; _minY *= 10.0f; _maxY *= 10.0f;
}

std::set<Body *> * Body::GetHit(void)
{
	if(_boundingNode)
	{
		_boundingNode->rem(this);
		return &_boundingNode->bodies();
	}
	else
	{
		return 0;
	}
}