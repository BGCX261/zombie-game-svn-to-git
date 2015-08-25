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
#include <glew.h>

#include "AM_Variable.hpp"
#include "AM_Shader.hpp"

#include "AM_Program.hpp"

namespace AutoMatic
{
	Program::Program(void)
		: _program(0), _linked(false)
	{
		_program = glCreateProgram();
		if(!_program){ throw Program_Initialisation_Failed; }
	}

	Program::~Program(void)
	{
		for (std::map<std::string, Variable *>::iterator i = _variables.begin(); i != _variables.end(); ++i)
		{
			delete (*i).second;
		}

		for (std::list<Shader *>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
		{
			glDetachShader(_program, (*i)->shader());
		}

		glDeleteProgram(_program);
	}

	void Program::attach(Shader * const & shader_)
	{
		glAttachShader(_program, shader_->shader());
		_shaders.push_back(shader_);
	}

	void Program::detach(Shader * const & shader_)
	{
		std::list<Shader *>::iterator i = std::find(_shaders.begin(), _shaders.end(), shader_);
		if(i == _shaders.end()){ return; }

		glDetachShader(_program, (*i)->shader());
		_shaders.erase(i);

		_linked = false;
	}

	void Program::link(void)
	{
		if(!_linked)
		{
			for (std::list<Shader *>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
			{
				(*i)->compile();
			}

			glLinkProgram(_program);

			GLint linkedOk = GL_FALSE;
			glGetProgramiv(_program, GL_LINK_STATUS, &linkedOk);
			if(linkedOk != GL_TRUE){ throw Program_Link_Failed; }

			_linked = true;
		}
	}

	void Program::use(void)
	{
		link();
		glUseProgram(_program);
	}

	void Program::log(std::string & logTo_)
	{
		GLint logLen = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0)
		{
			char * logPtr = new char[logLen + 1];
			GLint len = 0;
			glGetProgramInfoLog(_program, logLen, &len, logPtr);
			logPtr[len] = 0;
			if(len > 0){ logTo_ = logPtr; }
			delete[] logPtr;
		}
	}

	Variable * const & Program::uniformVariable(std::string const & id_)
	{
		link();
		
		Variable* & u = _variables[id_];
		if(!u){ u = new UniformVariable(id_, this); }
		return u;	

		// A similar set of function is available for data type integer, where "f" is replaced by "i"
		// For sampler variables, use the functions (OpenGL 2.0 notation) glUniform1i, 
		// or glUniform1iv if setting an array of samplers. 
		// Samples: A set of special types are available for texture access. 
		// These are required to access texture values, also known as texels. 
		// The data types for texture sampling are:

		//* sampler1D - for 1D textures
		//* sampler2D - for 2D textures
		//* sampler3D - for 3D textures
		//* samplerCube - for cube map textures
		//* sampler1DShadow - for shadow maps
		//* sampler2DShadow - for shadow maps


		//transpose - whether to transpose the matrix values.
		//A value of 1 indicates that the matrix values are specified in row major order, zero is column major order.
		// GLint glUniformMatrix{2,3,4}fv(GLint location, GLsizei count, GLboolean transpose, GLfloat *v);

		// The values that are set with these functions will keep their values until the program is linked again,
		// (or changed by these functions?)
		// Once a new link process is performed all values will be reset to zero.

		/*
		GLint loc1,loc2,loc3,loc4;
		float specIntensity = 0.98;
		float sc[4] = {0.8,0.8,0.8,1.0};
		float threshold[2] = {0.5,0.25};
		float colors[12] = {0.4,0.4,0.8,1.0,
		0.2,0.2,0.4,1.0,
		0.1,0.1,0.1,1.0};

		loc1 = glGetUniformLocationARB(p,"specIntensity");
		glUniform1fARB(loc1,specIntensity);

		loc2 = glGetUniformLocationARB(p,"specColor");
		glUniform4fvARB(loc2,1,sc);

		loc3 = glGetUniformLocationARB(p,"t");
		glUniform1fvARB(loc3,2,threshold);

		loc4 = glGetUniformLocationARB(p,"colors");
		glUniform4fvARB(loc4,3,colors);
		*/

		// Notice the difference between setting an array of values, as it is the case of t or colors, 
		// and setting a vector with 4 values, as the specColor. 
		// The count parameter (middle parameter of glGetUniform{1,2,3,4}fv) specifies 
		// the number of array elements as declared in the shader, not as declared in the OpenGL application. 
		// So although specColor contains 4 values, the count of the function glUniform4fv parameter is set to 1, 
		// because it is only one vector.

		//Yet another possibility provided by GLSL is to get the location of a variable inside an array. 
		// For instance, it is possible to get the location of t[1]. 
		// The following snippet of code shows this approach to set the t array elements.
		/*
		loct0 = glGetUniformLocation(p,"t[0]");
		glUniform1f(loct0,threshold[0]);

		loct1 = glGetUniformLocation(p,"t[1]");
		glUniform1f(loct1,threshold[1]);
		*/
	}

	Variable * const & Program::attributeVariable(std::string const & id_)
	{
		link();

		Variable* & a = _variables[id_];
		if(!a){ a = new AttributeVariable(id_, this); }
		return a;	

		/*


		A small example is now provided. It is assumed that the vertex shader declare a float attribute named height. The setup phase, to be performed after program link is:

		{
		loc = glGetAttribLocation(p,"height");
		}

		In the rendering function the code could be something like:

		{
		glBegin(GL_TRIANGLE_STRIP);

		glVertexAttrib1f(loc,2.0);
		glVertex2f(-1,1);

		glVertexAttrib1f(loc,2.0);
		glVertex2f(1,1);

		glVertexAttrib1f(loc,-2.0);
		glVertex2f(-1,-1);

		glVertexAttrib1f(loc,-2.0);
		glVertex2f(1,-1);

		glEnd();
		}

		*/
	}
}
