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

#ifndef _AM_GUI_hpp_
#define _AM_GUI_hpp_

#include "AM_InputHandler.hpp"

namespace Ogre
{
	class RenderWindow;
	class SceneManager;
}

namespace CEGUI
{
	class OgreCEGUIRenderer;
	class System;
	class Window;
}

namespace AutoMatic
{
	class Text
	{
	public:
		Text(std::string const & text_,  std::string const & name_, CEGUI::Window * parent_);

		void setPosition();
		void setText(std::string const & text_);

	private:
		CEGUI::Window * _parent;
		CEGUI::Window * _ceguiText;
	};

	class GUI : public InputCallback
	{
	public:
		GUI(Ogre::RenderWindow * window_, Ogre::SceneManager * sceneManager_, unsigned int priority_, unsigned int maxQuads_ = 3000);

		~GUI(void);

		void setSceneManager(Ogre::SceneManager * sceneManager_);

		void load(std::string const & scheme_ = "TaharezLookSkin.scheme", 
							std::string const & imageset_ = "TaharezLook",
							std::string const & defaultFont_ = "BlueHighway-10.font");

		virtual bool mouseMoved(OIS::MouseEvent const & mouseEvent_);

	private:
		CEGUI::OgreCEGUIRenderer* _renderer;
		CEGUI::System * _system;
		CEGUI::Window * _mainWindow;
	};
}

#endif // _AM_GUI_hpp_