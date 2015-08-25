/*
Copyright (c) 2007 Matthew Chudleigh matthewtoday@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _AM_GLFW_hpp_
#define _AM_GLFW_hpp_

#include "AM_Component.hpp"
#include "AM_TimeStep.hpp"

namespace AutoMatic
{
//****************************************************************************//

	class FramesPerSecond
	{
	public:
		FramesPerSecond(double refreshFrequency_)
			: _frames(0.0), _totalTime(0.0), _currTimePassed(0.0), _fps(0.0),
				_refreshFrequency(refreshFrequency_) {}

		bool update(double timestep_);

		double totalTime(void) const { return _totalTime; }
		double fps(void) const { return _fps; }

	private:
		double _frames, _totalTime, _currTimePassed, _fps, _refreshFrequency;
	};

//****************************************************************************//

	class GlfwWindow;
	class GLFW
	{
	public:
		enum Exception { No_Main_Instance, Window_Not_Set, Initialisation_Failed };

		GLFW(void);
		virtual ~GLFW(void);

		GlfwWindow & window(void);

		void loop(bool resetTimer_ = true);

		void continueLoop(bool continue_);

		void maxIdleDt(double maxIdleDt_);
		void minIdleDt(double minIdleDt_);

		double time(void) const;
		void time(double time_);

		FramesPerSecond const & fps(void) const { return _fps; }

		virtual void update(double timestep_, double timeLeft_) = 0;

		virtual void windowClosedCB(void) = 0;

		TimeStep & timeStep(void){ return _timeStep; }

	private:
		static int CloseWindowCB(void);
		static GLFW * _MainInstance;

		GlfwWindow * _window;

		bool _continueLoop;

		double _currTime, _minIdleDt, _maxIdleDt, _accum;
		FramesPerSecond _fps;
		TimeStep _timeStep;

		friend class GlfwWindow;
	};

//****************************************************************************//
}

#endif //_AM_GLFW_hpp_