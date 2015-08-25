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

#include <OgreCEGUIRenderer.h>
#include <cegui/CEGUIWindowManager.h>
#include <cegui/CEGUISchemeManager.h>
#include <cegui/CEGUISystem.h>
#include <cegui/CEGUIWindow.h>

#include "AM_GUI.hpp"

namespace AutoMatic
{
//****************************************************************************//

	Text::Text(std::string const & text_,  std::string const & name_, CEGUI::Window * parent_)
		: _parent(parent_)
	{
		_ceguiText = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Zombie/StaticText", (CEGUI::utf8*)name_.c_str());
		_ceguiText->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f,0), CEGUI::UDim(0.1f,0)));
		_ceguiText->setText((CEGUI::utf8*)text_.c_str());

		_parent->addChildWindow(_ceguiText);
		_ceguiText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	}

	void Text::setPosition()
	{

	}

	void Text::setText(std::string const & text_)
	{
		_ceguiText->setText((CEGUI::utf8*)text_.c_str());
	}

//****************************************************************************//

	GUI::GUI(Ogre::RenderWindow * window_, Ogre::SceneManager * sceneManager_, unsigned int priority_, unsigned int maxQuads_)
		: InputCallback(priority_), _renderer(0), _system(0), _mainWindow(0)
	{
		_renderer = new CEGUI::OgreCEGUIRenderer(window_, Ogre::RENDER_QUEUE_OVERLAY, false, maxQuads_, sceneManager_);
		_system = new CEGUI::System(_renderer);
	}

	GUI::~GUI(void)
	{
		delete _system;
		delete _renderer;
	}

	void GUI::setSceneManager(Ogre::SceneManager * sceneManager_)
	{
		_renderer->setTargetSceneManager(sceneManager_);
	}

	void GUI::load(std::string const & scheme_, std::string const & imageset_, std::string const & defaultFont_)
	{
		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)scheme_.c_str());
		_system->setDefaultMouseCursor((CEGUI::utf8*)imageset_.c_str(), (CEGUI::utf8*)"MouseArrow");
		_system->setDefaultFont(defaultFont_);
		_mainWindow= CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"Sheet");  
		_system->setGUISheet(_mainWindow);

		Text t("TestTestTestTestTestTestTestTestTestTestTestTestTest", "TestText", _mainWindow);
	}

	bool GUI::mouseMoved(OIS::MouseEvent const & mouseEvent_)
	{
		return _system->injectMousePosition(mouseEvent_.state.X.abs, mouseEvent_.state.Y.abs);
	}
}
