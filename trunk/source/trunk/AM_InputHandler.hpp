/*
Zombie Project.  A game.  With Zombies.
Copyright (C) 2007  Matthew Chudleigh

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _AM_InputHandler_hpp_
#define _AM_InputHandler_hpp_

#include <set>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

namespace AutoMatic
{
	/// 
	/** Not inherited by the [Key/Mouse]Callback classes, because that would caused Dreaded Diamond.
	*** The Callbacks return a boolean, true if the callback was handled and should NOT be passed to 
	*** other callbacks, false if the callback should be passed on.
	*/
	class InputCallback
	{
	public:
		InputCallback(unsigned int const priority_)
			: _priority(priority_)
		{}

		void priority(unsigned int const priority_){ _priority = priority_;	}
		unsigned int const priority(void) const { return _priority; }

		virtual bool keyPressed(OIS::KeyEvent const & keyboardEvent_){ return false; }
		virtual bool keyReleased(OIS::KeyEvent const & keyboardEvent_){ return false; }
		virtual bool mouseMoved(const OIS::MouseEvent & mouseEvent_){ return false; }
		virtual bool mousePressed(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_){ return false; }
		virtual bool mouseReleased(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_){ return false; }

	private:
		unsigned int _priority; /**< A greater number means a higher priority **/
	};

	class InputHandler 
		: public OIS::MouseListener
		, public OIS::KeyListener
	{
	public:
		InputHandler(unsigned long hWnd_); 
		~InputHandler(void);

		/// 
		/** \todo The \a timestep_ is not currently used. 
		*** \note The \a timestep_ is in milli-seconds.
		*/
		void update(unsigned long timestep_);
		void setWindowExtents(int width_, int height_);
		
		bool addInputCallBack(InputCallback * inputCallback_);
		bool mouseMoved(const OIS::MouseEvent & mouseEvent_);
		bool mousePressed(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_);
		bool mouseReleased(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_);
		bool keyPressed(const OIS::KeyEvent & keyboardEvent_);
		bool keyReleased(const OIS::KeyEvent & keyboardEvent_);

	private:
		typedef std::pair<unsigned int, InputCallback *> InputPair;
		typedef std::set< std::pair<unsigned int, InputCallback *> > InputCBSet;

		OIS::InputManager * _ois;
		OIS::Mouse * _mouse;
		OIS::Keyboard * _keyboard;

		InputCBSet _inputCallbacks;
	};
}

#endif //_AM_InputHandler_hpp_