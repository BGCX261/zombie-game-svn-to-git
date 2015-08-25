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

#ifndef ARBITER_H
#define ARBITER_H

#include "MathUtils.h"

struct Body;

struct FeaturePair
{
	union
	{
		struct Edges
		{
			char inEdge1;
			char outEdge1;
			char inEdge2;
			char outEdge2;
		} e;
		int key;
	};
};

struct Contact
{
	Contact() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {}

	Vec2 position;
	Vec2 normal;
	Vec2 r1, r2;
	float separation;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
	float massNormal, massTangent;
	float bias;
	FeaturePair feature;
};

struct Arbiter
{
	enum {MAX_POINTS = 2};

	Arbiter(Body* b1, Body* b2);

	void Update(Contact* contacts, int numContacts);

	void PreStep(float inv_dt);
	void ApplyImpulse();

	Contact _contacts[MAX_POINTS];
	int _numContacts;
	Body* _body1;
	Body* _body2;

	// Combined friction
	float _friction;
	bool _wasUpdated;
};

// This is used by std::set
inline bool operator < (const Arbiter& a1, const Arbiter& a2)
{
	if (a1._body1 < a2._body1)
		return true;

	if (a1._body1 == a2._body1 && a1._body2 < a2._body2)
		return true;

	return false;
}

int Collide(Contact* contacts, Body* body1, Body* body2);

#endif
