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

#include <glew.h>

#include "AM_Program.hpp"
#include "AM_Variable.hpp"

namespace AutoMatic
{
	Variable::Variable(std::string const & id_, Program * program_)
		: _location(-1)
	{
	}

	UniformVariable::UniformVariable(std::string const & id_, Program * program_)
		: Variable(id_, program_)
	{
		_location = glGetUniformLocation(program_->program(), id_.c_str());
		if(_location < 0){ throw Variable_Does_Not_Exist; }
	}

	void UniformVariable::set(GLfloat f0_)
	{
		glUniform1f(_location, f0_);
	}
	void UniformVariable::set(GLfloat f0_, GLfloat f1_)
	{
		glUniform2f(_location, f0_, f1_);
	}
	void UniformVariable::set(GLfloat f0_, GLfloat f1_, GLfloat f2_)
	{
		glUniform3f(_location, f0_, f1_, f2_);
	}
	void UniformVariable::set(GLfloat f0_, GLfloat f1_, GLfloat f2_, GLfloat f3_)
	{
		glUniform4f(_location,f0_, f1_, f2_, f3_);
	}

	void UniformVariable::set(GLfloat * fv_, int param_, int count_)
	{
		switch(param_)
		{
		case 1: { glUniform1fv(_location, count_, fv_); }
						break;
		case 2: { glUniform2fv(_location, count_, fv_); }
						break;
		case 3: { glUniform3fv(_location, count_, fv_); }
						break;
		case 4: { glUniform4fv(_location, count_, fv_); }
						break;
		default:
			break;
		}
	}

	void UniformVariable::set(GLint i0_)
	{
		glUniform1i(_location, i0_);
	}
	void UniformVariable::set(GLint i0_, GLint i1_)
	{
		glUniform2i(_location, i0_, i1_);
	}
	void UniformVariable::set(GLint i0_, GLint i1_, GLint i2_)
	{
		glUniform3i(_location, i0_, i1_, i2_);
	}
	void UniformVariable::set(GLint i0_, GLint i1_, GLint i2_, GLint i3_)
	{
		glUniform4i(_location,i0_, i1_, i2_, i3_);
	}

	void UniformVariable::set(GLint * iv_, int param_, int count_)
	{
		switch(param_)
		{
		case 1: { glUniform1iv(_location, count_, iv_); }
						break;
		case 2: { glUniform2iv(_location, count_, iv_); }
						break;
		case 3: { glUniform3iv(_location, count_, iv_); }
						break;
		case 4: { glUniform4iv(_location, count_, iv_); }
						break;
		default:
			break;
		}
	}

//****************************************************************************//

	AttributeVariable::AttributeVariable(std::string const & id_, Program * program_)
		: Variable(id_, program_)
	{
		_location = glGetAttribLocation(program_->program(), id_.c_str());
		if(_location < 0){ throw Variable_Does_Not_Exist; }
	}

	void AttributeVariable::set(GLfloat f0_)
	{
		glVertexAttrib1f(_location, f0_);
	}
	void AttributeVariable::set(GLfloat f0_, GLfloat f1_)
	{
		glVertexAttrib2f(_location, f0_, f1_);
	}
	void AttributeVariable::set(GLfloat f0_, GLfloat f1_, GLfloat f2_)
	{
		glVertexAttrib3f(_location, f0_, f1_, f2_);
	}
	void AttributeVariable::set(GLfloat f0_, GLfloat f1_, GLfloat f2_, GLfloat f3_)
	{
		glVertexAttrib4f(_location,f0_, f1_, f2_, f3_);
	}

	void AttributeVariable::set(GLfloat * fv_, int param_, int count_)
	{
		switch(param_)
		{
		case 1: { glVertexAttrib1fv(_location, fv_); }
						break;
		case 2: { glVertexAttrib2fv(_location, fv_); }
						break;
		case 3: { glVertexAttrib3fv(_location, fv_); }
						break;
		case 4: { glVertexAttrib4fv(_location, fv_); }
						break;
		default:
			break;
		}
	}

	void AttributeVariable::set(GLint i0_)
	{
		assert(false);
		//glVertexAttrib1i(_location, i0_);
	}
	void AttributeVariable::set(GLint i0_, GLint i1_)
	{
		assert(false);
		//glVertexAttrib2i(_location, i0_, i1_);
	}
	void AttributeVariable::set(GLint i0_, GLint i1_, GLint i2_)
	{
		assert(false);
		//glVertexAttrib3i(_location, i0_, i1_, i2_);
	}
	void AttributeVariable::set(GLint i0_, GLint i1_, GLint i2_, GLint i3_)
	{
		assert(false);
		//glVertexAttrib4i(_location,i0_, i1_, i2_, i3_);
	}

	void AttributeVariable::set(GLint * iv_, int param_, int count_)
	{
		switch(param_)
		{
		case 1: 
			{ 
				assert(false);
				//	glVertexAttrib1iv(_location, count_, iv_); 
			}
			break;
		case 2: 
			{ 
				assert(false);
				//	glVertexAttrib2iv(_location, count_, iv_); 
			}
			break;
		case 3: 
			{ 
				assert(false);
				//	glVertexAttrib3iv(_location, count_, iv_); 
			}
			break;
		case 4: 
			{ 
				glVertexAttrib4iv(_location, iv_); 
			}
			break;
		default:
			break;
		}
	}
}