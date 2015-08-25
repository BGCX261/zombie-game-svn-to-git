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

#ifndef OPENSTEER_SIMPLEVEHICLEANNOTATED_H
#define OPENSTEER_SIMPLEVEHICLEANNOTATED_H

#include "SimpleVehicle.h"
#include "Annotation.h"

namespace OpenSteer
{
	// SimpleVehicle_2 adds concrete annotation methods to SimpleVehicle_1
	typedef AnnotationMixin<SimpleVehicle> SimpleVehicle_3;

	class SimpleVehicleAnnotated : public SimpleVehicle_3
	{
	public:
		SimpleVehicleAnnotated(void)
			: gaudyPursuitAnnotation(false) 
		{
			reset();
		}

		void reset(void);

		// for annotation
		bool gaudyPursuitAnnotation;

		// draw lines from vehicle's position showing its velocity and acceleration
		void annotationVelocityAcceleration (float maxLengthA, float maxLengthV);
		void annotationVelocityAcceleration (float maxLength)
		{annotationVelocityAcceleration (maxLength, maxLength);}
		void annotationVelocityAcceleration (void)
		{annotationVelocityAcceleration (3, 3);}

		Vec3 steerForPursuit(const AbstractVehicle& quarry, const float maxPredictionTime);
	};
}

#endif // OPENSTEER_SIMPLEVEHICLEANNOTATED_H
