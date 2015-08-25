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

#include <glew.h>

#include "AM_Shader.hpp"

namespace AutoMatic
{
//****************************************************************************//

	Shader::Shader(GLuint shader_, char const * shaderData_, bool compile_)
		: _shader(shader_), _shaderData(shaderData_), _compiled(false)
	{
		if(!_shader){ throw Invalid_Shader_Handle; }
		if(!_shaderData){ throw Invalid_Shader_Data; }
		if(compile_){ compile(); }
	}

	Shader::~Shader(void)
	{
		delete[] _shaderData;
		glDeleteShader(_shader);
	}

	void Shader::compile(void)
	{
		if(!_compiled)
		{
			glShaderSource(_shader, 1, &_shaderData, 0);
			glCompileShader(_shader);

			GLint compiledOk = GL_FALSE;
			glGetShaderiv(_shader, GL_COMPILE_STATUS, &compiledOk);
			if(compiledOk != GL_TRUE){ throw Shader_Compile_Failed; }

			_compiled = true;
		}
	}

	void Shader::log(std::string & logTo_)
	{
		GLint logLen = 0;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0)
		{
			char * logPtr = new char[logLen + 1];
			GLint len = 0;
			glGetShaderInfoLog(_shader, logLen, &len, logPtr);
			logPtr[len] = 0;
			if(len > 0){ logTo_ = logPtr; }
			delete[] logPtr;
		}
	}

//****************************************************************************//

	FragmentShader::FragmentShader(char const * shaderData_, bool compile_)
		: Shader( glCreateShader(GL_FRAGMENT_SHADER), shaderData_, compile_)
	{}

	VertexShader::VertexShader(char const * shaderData_, bool compile_)
		: Shader( glCreateShader(GL_VERTEX_SHADER), shaderData_, compile_)
	{}


//****************************************************************************//
}
