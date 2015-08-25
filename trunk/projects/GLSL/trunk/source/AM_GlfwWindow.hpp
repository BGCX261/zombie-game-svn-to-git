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

#ifndef _AM_GlfwWindow_hpp_
#define _AM_GlfwWindow_hpp_

#include <string>

namespace AutoMatic
{
	class GlfwWindow
	{
	public:
		enum Exception { GlfwWindow_Initialisation_Failed };
		enum Mode { Windowed, Fullscreen };

		GlfwWindow(void);
		~GlfwWindow(void);

		void open(int width_, int height_, int depth_, Mode mode_, std::string const & title_);
		void close(void);

		bool isOpen(void) const;

	private:
		bool _isOpen;
	};
}

#endif //_AM_GlfwWindow_hpp_
