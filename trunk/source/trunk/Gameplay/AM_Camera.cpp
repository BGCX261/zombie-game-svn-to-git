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

#include <Ogre.h>

#include "AM_Camera.hpp"

namespace AutoMatic
{
	Camera::Camera(std::string const & name_, Ogre::SceneManager * sceneManager_, Ogre::RenderWindow * window_
		, float left_, float top_, float width_, float height_)
		: _camera(0), _viewport(0)
	{
		_camera = sceneManager_->createCamera(name_);
		_viewport = window_->addViewport(_camera, 0, left_, top_, width_, height_);

//		_camera->setPosition(Ogre::Vector3(0.0f,0.0f,0.0f));
		//_camera->lookAt(Ogre::Vector3(0.0f,0.0f,-1.0f));

		_camera->setPosition(Ogre::Vector3(0.0f,10.0f,15.0f));
		_camera->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));

		_camera->setFarClipDistance(10000.0f);
		_camera->setNearClipDistance(5.0f);

		_camera->setAspectRatio( Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));
		_viewport->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
	}

	void Camera::position(float x_, float y_, float z_)
	{
		_camera->setPosition(x_, y_, z_);
	}

	void Camera::target(float x_, float y_, float z_)
	{
		_camera->lookAt(x_, y_, z_);
	}
}
