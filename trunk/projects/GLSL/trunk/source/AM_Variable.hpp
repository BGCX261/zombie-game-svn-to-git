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

#ifndef _AM_Variable_hpp_
#define _AM_Variable_hpp_

#include <string>

#include <gl.h>

namespace AutoMatic
{
	class Program;
	class Variable
	{
	public:
		enum Exception { Variable_Does_Not_Exist };

		Variable(std::string const & id_, Program * program_);
		virtual ~Variable(void){}

		/// Set a single float value for this variable.
		virtual void set(GLfloat f0_) = 0;
		/// Set two float values for this variable (vec2)
		virtual void set(GLfloat f0_, GLfloat f1_) = 0;
		/// Set three float values for this variable (vec3)
		virtual void set(GLfloat f0_, GLfloat f1_, GLfloat f2_) = 0;
		/// Set four float values for this variable (vec4)
		virtual void set(GLfloat f0_, GLfloat f1_, GLfloat f2_, GLfloat f3_) = 0;

		/// Set an array of floats.
		/** \param fv_ An array (possibly of arrays)
			* \param param_ The number of float values per array.
			* \param count_ The number of arrays. 
			* \todo Should \a count_ default to 0, or 1?
		*/
		virtual void set(GLfloat * fv_, int param_, int count_ = 0) = 0;

		virtual void set(GLint i0_) = 0;
		virtual void set(GLint i0_, GLint i1_) = 0;
		virtual void set(GLint i0_, GLint i1_, GLint i2_) = 0;
		virtual void set(GLint i0_, GLint i1_, GLint i2_, GLint i3_) = 0;

		virtual void set(GLint * iv_, int param_, int count_ = 0) = 0;

	protected:
		GLint _location;
	};

	class UniformVariable : public Variable
	{
	public:
		UniformVariable(std::string const & id_, Program * program_);

		virtual void set(GLfloat f0_);
		virtual void set(GLfloat f0_, GLfloat f1_);
		virtual void set(GLfloat f0_, GLfloat f1_, GLfloat f2_);
		virtual void set(GLfloat f0_, GLfloat f1_, GLfloat f2_, GLfloat f3_);

		virtual void set(GLfloat * fv_, int param_, int count_ = 0);

		virtual void set(GLint i0_);
		virtual void set(GLint i0_, GLint i1_);
		virtual void set(GLint i0_, GLint i1_, GLint i2_);
		virtual void set(GLint i0_, GLint i1_, GLint i2_, GLint i3_);

		virtual void set(GLint * iv_, int param_, int count_ = 0);
	};

	class AttributeVariable : public Variable
	{
	public:
		AttributeVariable(std::string const & id_, Program * program_);

		virtual void set(GLfloat f0_);
		virtual void set(GLfloat f0_, GLfloat f1_);
		virtual void set(GLfloat f0_, GLfloat f1_, GLfloat f2_);
		virtual void set(GLfloat f0_, GLfloat f1_, GLfloat f2_, GLfloat f3_);

		/// 
		/** Note that the vector version is not available for arrays as is the case of uniform variables. 
		*** The vector version is just an option to submit the values of a single attribute variable. 
			* This is similar to what happens in OpenGL with glColor3f and glColor3fv.
		*/
		virtual void set(GLfloat * fv_, int param_, int count_ = 0);

		virtual void set(GLint i0_);
		virtual void set(GLint i0_, GLint i1_);
		virtual void set(GLint i0_, GLint i1_, GLint i2_);
		virtual void set(GLint i0_, GLint i1_, GLint i2_, GLint i3_);

		/// 
		/** Note that the vector version is not available for arrays as is the case of uniform variables. 
		*** The vector version is just an option to submit the values of a single attribute variable. 
		* This is similar to what happens in OpenGL with glColor3f and glColor3fv.
		*/
		virtual void set(GLint * iv_, int param_, int count_ = 0);
	};

	/* TODO: 
	Vertex Arrays can also be used together with attribute variables. 
	The first thing to be done is to enable the arrays. 
	To do this for an attribute array use the following function (OpenGL 2.0 syntax):

	{
	void glEnableVertexAttribArray(GLint loc);
	}

	Parameters:
	loc - the location of the variable. 

	Next the pointer to the array with the data is provided using the following functions.

	OpenGL 2.0 syntax:

	{
	void glVertexAttribPointer(GLint loc, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
	}

	Parameters:
	loc - the location of the variable.
	size - the number of components per element, for instance: 1 for float; 2 for vec2; 3 for vec3, and so on.
	type - The data type associated: GL_FLOAT is an example.
	normalized - if set to 1 then the array values will be normalized, converted to a range from -1 to 1 for signed data, or 0 to 1 for unsigned data.
	stride - the spacing between elements. Exactly the same as in OpenGL.
	pointer - pointer to the array containing the data. 

	{
	float vertices[8] = {-1,1, 1,1, -1,-1, 1,-1};
	float heights[4] = {2,2,-2,-2};

	...

	loc = glGetAttribLocationARB(p,"height");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArrayARB(loc);

	glVertexPointer(2,GL_FLOAT,0,vertices);
	glVertexAttribPointerARB(loc,1,GL_FLOAT,0,0,heights);
	}

	When glDrawArrays(GL_TRIANGLE_STRIP,0,4); is called, the heights[] array will
	be used in the shader.  For each triangle vertex (2-coord pair) 1 height will be used:
	{-1,1,2}, {1,1,2}, {-1,-1,-2}, {1,-1,-2}
	*/
}

#endif //_AM_Variable_hpp_