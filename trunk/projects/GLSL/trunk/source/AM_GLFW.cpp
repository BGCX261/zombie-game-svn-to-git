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

#include <cassert>
#include <glfw.h>

#include "AM_GLFWInput.hpp"
#include "AM_GLFWWindow.hpp"
#include "AM_GLFW.hpp"

#include <AM_Memory.h>

namespace AutoMatic
{
//****************************************************************************//

	bool FramesPerSecond::update(double timestep_)
	{
		_currTimePassed += timestep_;
		_totalTime += timestep_;

		if(_currTimePassed >= _refreshFrequency)
		{
			_fps = _frames / _currTimePassed;
			_currTimePassed = 0.0; _frames = 0.0;
			return true;
		}

		return false;
	}

//****************************************************************************//

	GLFW * GLFW::_MainInstance = 0;

	GLFW::GLFW(void)
		: _continueLoop(true), _currTime(0.0), _minIdleDt(0.1), _maxIdleDt(2.0), _accum(0.0),
			_fps(2.0), _window(0)
	{	
		if(!_MainInstance)
		{
			int ok = glfwInit();
			if(GL_FALSE == ok){ throw Initialisation_Failed; }
			_window = new GlfwWindow;
			_MainInstance = this;
		}
	}

	GLFW::~GLFW(void)
	{
		if(this == _MainInstance)
		{
			delete _window;
			glfwTerminate();
			_MainInstance = 0;
		}
	}

	GlfwWindow & GLFW::window(void)
	{
		return *_MainInstance->_window;
	}

	void  GLFW::maxIdleDt(double maxIdleDt_)
	{
		_maxIdleDt = maxIdleDt_;
	}

	void GLFW::minIdleDt(double minIdleDt_)
	{
		_minIdleDt = minIdleDt_;
	}

	void GLFW::loop(bool resetTimer_)
	{
		if(resetTimer_){ time(0.0); }
		while(_continueLoop)
		{
			double t = time();
			double dt = t - _currTime;
			double currDt = _minIdleDt;
			_currTime = t;

			if(dt > _maxIdleDt){ dt = _maxIdleDt; }

			_accum += dt;
			if(_accum >= _minIdleDt)
			{
				while(_accum > 0.0)
				{
					_accum -= currDt;
					
					if(_accum < 0.0)
					{ 
						currDt += _accum;
						_accum = 0.0; 
					}

					_timeStep.set(currDt);

					update(currDt, _accum);
				}
			}
			else
			{
				update(0.0, 0.0);
			}

			// _accum / dt; // Remainder time, use to interpolate.
			
			_fps.update(dt);
		}
	}

	void GLFW::continueLoop(bool continue_)
	{
		_continueLoop = continue_;
	}

	double GLFW::time(void) const
	{
		return glfwGetTime();
	}

	void GLFW::time(double time_)
	{
		glfwSetTime(time_);
	}

	int GLFW::CloseWindowCB(void)
	{
		_MainInstance->windowClosedCB();
		return GL_TRUE;
	}

//****************************************************************************//
}
