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

#include <algorithm>
#include <cassert>
#include <glew.h>

#include "AM_Program.hpp"
#include "AM_Shader.hpp"
#include "AM_GLSL.hpp"

namespace AutoMatic
{

//****************************************************************************//

	bool GLSL::_GlewInitialised = false;

	GLSL::GLSL(void)
	{
		if(!_GlewInitialised)
		{
			GLenum err = glewInit();
			if( err != GLEW_OK ){ throw Glew_Initialisation_Failed; }

			if (!glewIsSupported("GL_VERSION_2_0"))
			{
				throw OpenGL2_Not_Supported;
			}
			_GlewInitialised = true;
		}
	}

	GLSL::~GLSL(void)
	{
		for (std::map<std::string, Program *>::iterator i = _programs.begin(); i != _programs.end(); ++i)
		{ // Must go first, to detach all of the shader's from the programs.
			delete (*i).second;
		}

		for (std::map<std::string, Shader *>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
		{
			delete (*i).second;
		}
	}

	Shader * const & GLSL::fragmentShader(std::string const & shaderId_, char const * shaderData_, bool compile_)
	{
		Shader* & s = _shaders[shaderId_];
		if(!s){ s = new FragmentShader(shaderData_, compile_); }
		else if(compile_){ s->compile(); }
		return s;
	}

	Shader * const & GLSL::vertexShader(std::string const & shaderId_, char const * shaderData_, bool compile_)
	{
		Shader* & s = _shaders[shaderId_];
		if(!s){ s = new VertexShader(shaderData_, compile_); }
		else if(compile_){ s->compile(); }
		return s;
	}

	Program * const & GLSL::program(std::string const & id_)
	{
		Program* & p = _programs[id_];
		if(!p){ p = new Program; }
		return p;
	}

	void GLSL::disable(void)
	{
		glUseProgram(0);
	}

//****************************************************************************//

}
