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

#ifndef _AutoMatic_GLSL_hpp_
#define _AutoMatic_GLSL_hpp_

#include <map>
#include <list>
#include <string>
#include <gl.h>

namespace AutoMatic
{

//****************************************************************************//
	class Program;
	class Shader;
	/// A class for holding GLSL Shaders and Programs.
	class GLSL
	{
	public:
		enum Exception { Glew_Initialisation_Failed, OpenGL2_Not_Supported };

		GLSL(void);

		~GLSL(void);

		/// Get or create a FragmentShader.
		/** Shader's are stored in a map by their filename.  Calling fragmentShader 
			* more than once with the same \a filename_ will return the same shader.
			* GLSL maintains control over the pointer.  Do not delete it.
		*/
		Shader * const & fragmentShader(std::string const & shaderId_, char const * shaderData_, bool compile_ = false);

		/// Get or create a VertexShader.
		/** Shader's are stored in a map by their filename.  Calling vertexShader 
			* more than once with the same \a filename_ will return the same shader.
			* GLSL maintains control over the pointer.  Do not delete it.
		*/
		Shader * const & vertexShader(std::string const & shaderId_, char const * shaderData_, bool compile_ = false);

		/// Get or create a Program.
		/** GLSL maintains control over the pointer.  Do not delete it.
		*/
		Program * const & program(std::string const & id_);

		/// Disable all the shader programs.
		/** Simple call Program->use() to start a shader again.
		*/
		void disable(void);

	private:
		static bool _GlewInitialised; /**< Has GLEW been initialised? **/

		std::map<std::string, Program *> _programs; /**< All the programs that have been created. **/
		std::map<std::string, Shader *> _shaders; /**< All the shaders that have been created. **/
	};

//****************************************************************************//

}

#endif //_AutoMatic_GLSL_hpp_
