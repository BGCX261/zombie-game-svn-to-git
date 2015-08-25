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

#ifndef _AM_Program_hpp_
#define _AM_Program_hpp_

#include <map>
#include <list>

#include <gl.h>

namespace AutoMatic
{
	class UniformVariable;
	class AttributeVariable;
	class Variable;
	class Shader;
	class Program
	{
	public:
		enum Exception { Program_Initialisation_Failed, Program_Link_Failed};
		Program(void);

		/// A program should be deleted before the shader's that are still attached, to detach them.
		~Program(void);

		/// Attach a shader to this program.
		/** \note Currently the same shader can be added more than once,
			* and detach will remove the first of the same pointer that it comes across.
			* \note The pointer will be stored, but not controlled.
		*/
		void attach(Shader * const & shader_);

		/// Detach a shader from this program.
		/** \note Currently the same shader can be added more than once,
		* and detach will remove the first of the same pointer that it comes across.
		*/
		void detach(Shader * const & shader_);

		/// Compile and link all shader's into the program.
		/** After the link operation the shader's source can be modified,
			* and the shader's recompiled without affecting the program.
		*/
		void link(void);

		/// Make this program the current one.
		/** This will link before use, if not already.
		*/
		void use(void);

		/// Retrieve the program log.
		void log(std::string & logTo_);

		/// Retrieve a uniformVariable from the program.
		/** The Program maintains control over the pointer.
		*** A new UniformVariable will be created if it does not already exist.
		*/
		Variable * const & uniformVariable(std::string const & id_);

		/// Retrieve an attributeVariable from the program.
		/** The Program maintains control over the pointer.
		*** A new AttributeVariable will be created if it does not already exist.
		*/
		Variable * const & attributeVariable(std::string const & id_);

		/// Get the id of this program.
		GLuint program(void) const { return _program; }

	private:
		std::map<std::string, Variable *> _variables; /**< What variables are available in this program. **/
		std::list<Shader *> _shaders; /**< \todo Can the same shader be added more than once? 
																	If so, how do I handle attach/detach properly? **/
		bool _linked;	/**< Has the program been linked with its current shader's? **/
		GLuint _program; /**< The OpenGL Program Id of this program **/
	};
}

#endif //_AM_Program_hpp_