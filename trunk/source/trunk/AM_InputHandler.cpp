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

#include <OgreStringConverter.h>

#include "AM_InputHandler.hpp"

namespace AutoMatic
{
	InputHandler::InputHandler(unsigned long hWnd_)  
		: _ois(0), _mouse(0), _keyboard(0)
	{
		OIS::ParamList pl;
		pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd_)));

		_ois = OIS::InputManager::createInputSystem( pl );
		_mouse = static_cast<OIS::Mouse*>(_ois->createInputObject( OIS::OISMouse, true ));
		_keyboard = static_cast<OIS::Keyboard*>(_ois->createInputObject( OIS::OISKeyboard, true));
		
		_mouse->setEventCallback(this);
		_keyboard->setEventCallback(this);
	}

	InputHandler::~InputHandler(void) 
	{
		_ois->destroyInputObject(_mouse);
		_ois->destroyInputObject( _keyboard );

		OIS::InputManager::destroyInputSystem(_ois);
	}

	void InputHandler::update(unsigned long timestep_) 
	{
		_mouse->capture();
		_keyboard->capture();
	}

	void  InputHandler::setWindowExtents(int width_, int height_)
	{
		//Set Mouse Region.. if window resizes, we should alter this to reflect as well
		const OIS::MouseState & mouseState = _mouse->getMouseState();
		mouseState.width = width_;
		mouseState.height = height_;
	}

	bool InputHandler::addInputCallBack(InputCallback * inputCallback_)
	{ 
		if(!inputCallback_){ return false; }
		InputCBSet::size_type size = _inputCallbacks.size();
		_inputCallbacks.insert(InputPair(inputCallback_->priority(), inputCallback_) ); 
		return size < _inputCallbacks.size(); 
	}

	bool InputHandler::mouseMoved(const OIS::MouseEvent & mouseEvent_) 
	{
		for (InputCBSet::iterator i = _inputCallbacks.begin(); 
				 i != _inputCallbacks.end(); ++i)
		{
			if((*i).second->mouseMoved(mouseEvent_)){ return false; }
		}
		return true;
	}

	bool InputHandler::mousePressed(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_)
	{
		for (InputCBSet::iterator i = _inputCallbacks.begin(); 
				 i != _inputCallbacks.end(); ++i)
		{
			if((*i).second->mousePressed(mouseEvent_, mouseButton_)){ return false; }
		}
		return true;
	}

	bool InputHandler::mouseReleased(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_) 
	{
		for (InputCBSet::iterator i = _inputCallbacks.begin(); 
			i != _inputCallbacks.end(); ++i)
		{
			if((*i).second->mouseReleased(mouseEvent_, mouseButton_)){ return false; }
		}
		return true;
	}

	bool InputHandler::keyPressed(const OIS::KeyEvent & keyboardEvent_) 
	{
		for (InputCBSet::iterator i = _inputCallbacks.begin(); 
			i != _inputCallbacks.end(); ++i)
		{
			if((*i).second->keyPressed(keyboardEvent_)){ return false; }
		}
		return true;
	}

	bool InputHandler::keyReleased(const OIS::KeyEvent & keyboardEvent_)
	{
		for (InputCBSet::iterator i = _inputCallbacks.begin(); 
			i != _inputCallbacks.end(); ++i)
		{
			if((*i).second->keyReleased(keyboardEvent_)){ return false; }
		}
		return true;
	}
}
