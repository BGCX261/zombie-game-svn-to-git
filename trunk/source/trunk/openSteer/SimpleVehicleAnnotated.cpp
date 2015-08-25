// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------

#include "SimpleVehicleAnnotated.h"

namespace OpenSteer
{
// ----------------------------------------------------------------------------
	// draw lines from vehicle's position showing its velocity and acceleration
	void OpenSteer::SimpleVehicleAnnotated::reset(void)
	{
		SimpleVehicle_3::reset();
		// default to non-gaudyPursuitAnnotation
		gaudyPursuitAnnotation = false;
	}

	void 
		OpenSteer::SimpleVehicleAnnotated::annotationVelocityAcceleration (float maxLengthA, 
		float maxLengthV)
	{
		const float desat = 0.4f;
		const float aScale = maxLengthA / maxForce ();
		const float vScale = maxLengthV / maxSpeed ();
		const Vec3& p = position();
		const Vec3 aColor (desat, desat, 1); // bluish
		const Vec3 vColor (    1, desat, 1); // pinkish

		annotationLine (p, p + (velocity ()           * vScale), vColor);
		annotationLine (p, p + (_smoothedAcceleration * aScale), aColor);
	}

	OpenSteer::Vec3 OpenSteer::SimpleVehicleAnnotated::steerForPursuit(const OpenSteer::AbstractVehicle& quarry, 
		const float maxPredictionTime)
	{
		int forward, parallel;
		Vec3 target, colour;
		Vec3 ret = SimpleVehicle_2::steerForPursuit(quarry, maxPredictionTime, forward, parallel, target);

		switch (forward)
		{
		case +1:
			switch (parallel)
			{
			case +1:          // ahead, parallel
				colour = gBlack;
				break;
			case 0:           // ahead, perpendicular
				colour = gGray50;
				break;
			case -1:          // ahead, anti-parallel
				colour = gWhite;
				break;
			}
			break;
		case 0:
			switch (parallel)
			{
			case +1:          // aside, parallel
				colour = gRed;
				break;
			case 0:           // aside, perpendicular
				colour = gYellow;
				break;
			case -1:          // aside, anti-parallel
				colour = gGreen;
				break;
			}
			break;
		case -1:
			switch (parallel)
			{
			case +1:          // behind, parallel
				colour= gCyan;
				break;
			case 0:           // behind, perpendicular
				colour= gBlue;
				break;
			case -1:          // behind, anti-parallel
				colour = gMagenta;
				break;
			}
			break;
		}

		// annotation
		annotationLine (position(),
			target,
			gaudyPursuitAnnotation ? colour : gGray40);

		return ret;
	}
// ----------------------------------------------------------------------------
}
