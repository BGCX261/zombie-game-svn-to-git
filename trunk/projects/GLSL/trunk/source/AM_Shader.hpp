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

#ifndef _AM_Shader_hpp_
#define _AM_Shader_hpp_

#include <string>

#include <gl.h>

//****************************************************************************//
namespace AutoMatic
{
	class Shader
	{
	public:
		enum Exception { Invalid_Shader_Data, Invalid_Shader_Handle, Shader_Compile_Failed };

		/// Create a new shader.  
		/** This class should be created via a FragmentShader or VertexShader.
			* \note Shader takes control of the shaderData_ pointer (so it can delay compilation).
		*/
		Shader(GLuint shader_, char const * shaderData_, bool compile_ = false);

		/// Clean up the OpenGL shader and shaderData.
		/** In the case of an OpenGL shader that is still attached to some (one or more) programs, 
		* the (OpenGL) shader is not actually deleted, but only marked for deletion. 
		* The delete operation will only be concluded when the shader is no longer attached to any program, 
		* i.e. it has been detached from all programs it was attached to.
		*/
		virtual ~Shader(void);

		/// Compile the shader
		/** \throws Shader_Compile_Failed is compilation failed.
		*/
		void compile(void);

		GLuint shader(void) const { return _shader; }

		/// Retrieve the shader log.
		void log(std::string & logTo_);

	private:
		GLuint _shader;
		bool _compiled;
		char const * _shaderData;
	};

	//***********************************************************************//

	class FragmentShader : public Shader
	{
	public:
		FragmentShader(char const * shaderData_, bool compile_ = false);
	};

	//***********************************************************************//

	class VertexShader : public Shader
	{
	public:
		VertexShader(char const * shaderData_, bool compile_ = false);
	};

	//***********************************************************************//
}

#endif //_AM_Shader_hpp_