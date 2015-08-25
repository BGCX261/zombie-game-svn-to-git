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

#include "Arbiter.h"
#include "Body.h"

#include <windows.h>
#include <gl\gl.h>

#define BIAS_PRESERVES_MOMENTUM	1

Arbiter::Arbiter(Body* b1, Body* b2)
	: _wasUpdated(true)
{
	if (b1 < b2)
	{
		_body1 = b1;
		_body2 = b2;
	}
	else
	{
		_body1 = b2;
		_body2 = b1;
	}

	_numContacts = Collide(_contacts, _body1, _body2);

	_friction = sqrtf(_body1->friction * _body2->friction);

	{
		glPointSize(4.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < _numContacts; ++i)
		{
			glVertex2f(_contacts[i].position.x, _contacts[i].position.y);
		}
		glEnd();
		glPointSize(1.0f);
	}
}

void Arbiter::Update(Contact* newContacts_, int numNewContacts_)
{
	_wasUpdated = true;

	assert(numNewContacts_ <= 2);
	Contact mergedContacts[2];

	for (int i = 0; i < numNewContacts_; ++i)
	{ // For each of the new contacts
		Contact* cNew = newContacts_ + i;
		int k = -1;
		for (int j = 0; j < _numContacts; ++j)
		{ // For each of the existing contacts,
				// Search for an old contact with the same key
			Contact* cOld = _contacts + j;
			if (cNew->feature.key == cOld->feature.key)
			{ // If the keys are the same, a matching contact is found.
				k = j;
				break;
			}
		}

		if (k > -1)
		{ // At match was found
			Contact* c = mergedContacts + i;
			Contact* cOld = _contacts + k;
			*c = *cNew; // Copy the new contact to the old contact
			c->Pn = cOld->Pn; // Assign the OLD normal impulse
			c->Pt = cOld->Pt; // Assign the OLD tangent impulse
		}
		else
		{ // Not match was made, copy the new contact completely
			mergedContacts[i] = newContacts_[i];
		}
	}

	for (int i = 0; i < numNewContacts_; ++i)
	{
		_contacts[i] = mergedContacts[i];
	}

	_numContacts = numNewContacts_;
}


void Arbiter::PreStep(float inv_dt)
{
	const float k_allowedPenetration = 0.01f;
	const float k_biasFactor = 0.8f;

	for (int i = 0; i < _numContacts; ++i)
	{
		Contact* c = _contacts + i;

		Vec2 r1 = c->position - _body1->position;
		Vec2 r2 = c->position - _body2->position;

		// Precompute normal mass, tangent mass, and bias.
		float rn1 = Dot(r1, c->normal);
		float rn2 = Dot(r2, c->normal);
		float kNormal = _body1->invMass + _body2->invMass;
		kNormal += _body1->invI * (Dot(r1, r1) - rn1 * rn1) + _body2->invI * (Dot(r2, r2) - rn2 * rn2);
		c->massNormal = 1.0f / kNormal;

		Vec2 tangent = Cross(c->normal, 1.0f);
		float rt1 = Dot(r1, tangent);
		float rt2 = Dot(r2, tangent);
		float kTangent = _body1->invMass + _body2->invMass;
		kTangent += _body1->invI * (Dot(r1, r1) - rt1 * rt1) + _body2->invI * (Dot(r2, r2) - rt2 * rt2);
		c->massTangent = 1.0f /  kTangent;

		c->bias = -k_biasFactor * inv_dt * Min(0.0f, c->separation + k_allowedPenetration);

		// Apply normal + friction impulse
		Vec2 P = c->Pn * c->normal + c->Pt * tangent;

		_body1->velocity -= _body1->invMass * P;
		_body1->angularVelocity -= _body1->invI * Cross(r1, P);

		_body2->velocity += _body2->invMass * P;
		_body2->angularVelocity += _body2->invI * Cross(r2, P);

		// Initialize bias impulse to zero.
		c->Pnb = 0.0f;
	}
}

void Arbiter::ApplyImpulse()
{
	Body* b1 = _body1;
	Body* b2 = _body2;

	for (int i = 0; i < _numContacts; ++i)
	{
		Contact* c = _contacts + i;
		c->r1 = c->position - b1->position;
		c->r2 = c->position - b2->position;

		// Relative velocity at contact
		Vec2 dv = b2->velocity + Cross(b2->angularVelocity, c->r2) - b1->velocity - Cross(b1->angularVelocity, c->r1);

		// Compute normal impulse
		float vn = Dot(dv, c->normal);
#if BIAS_PRESERVES_MOMENTUM == 0
		float dPn = c->massNormal * (-vn + c->bias);
#else
		float dPn = c->massNormal * (-vn);
#endif
		// Clamp the accumulated impulse
		float Pn0 = c->Pn;
		c->Pn = Max(Pn0 + dPn, 0.0f);
		dPn = c->Pn - Pn0;

		// Apply contact impulse
		Vec2 Pn = dPn * c->normal;

		b1->velocity -= b1->invMass * Pn;
		b1->angularVelocity -= b1->invI * Cross(c->r1, Pn);

		b2->velocity += b2->invMass * Pn;
		b2->angularVelocity += b2->invI * Cross(c->r2, Pn);

#if BIAS_PRESERVES_MOMENTUM == 1
		// Compute bias impulse
		dv = b2->biasedVelocity + Cross(b2->biasedAngularVelocity, c->r2) - b1->biasedVelocity - Cross(b1->biasedAngularVelocity, c->r1);
		float vnb = Dot(dv, c->normal);

		float dPnb = c->massNormal * (-vnb + c->bias);
		float Pnb0 = c->Pnb;
		c->Pnb = Max(Pnb0 + dPnb, 0.0f);
		dPnb = c->Pnb - Pnb0;

		Vec2 Pb = dPnb * c->normal;

		b1->biasedVelocity -= b1->invMass * Pb;
		b1->biasedAngularVelocity -= b1->invI * Cross(c->r1, Pb);

		b2->biasedVelocity += b2->invMass * Pb;
		b2->biasedAngularVelocity += b2->invI * Cross(c->r2, Pb);
#endif

		// Relative velocity at contact
		dv = b2->velocity + Cross(b2->angularVelocity, c->r2) - b1->velocity - Cross(b1->angularVelocity, c->r1);

		// Compute friction impulse
		float maxPt = _friction * c->Pn;

		Vec2 tangent = Cross(c->normal, 1.0f);
		float vt = Dot(dv, tangent);
		float dPt = c->massTangent * (-vt);

		// Clamp friction
		float oldTangentImpulse = c->Pt;
		c->Pt = Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
		dPt = c->Pt - oldTangentImpulse;

		// Apply contact impulse
		Vec2 Pt = dPt * tangent;

		b1->velocity -= b1->invMass * Pt;
		b1->angularVelocity -= b1->invI * Cross(c->r1, Pt);

		b2->velocity += b2->invMass * Pt;
		b2->angularVelocity += b2->invI * Cross(c->r2, Pt);
	}
}
