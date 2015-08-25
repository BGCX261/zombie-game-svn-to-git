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

#ifndef _AM_Camera_hpp_
#define _AM_Camera_hpp_

#include <string>
	
#include "Serialisation/AM_Serialisation.hpp"

namespace Ogre
{
	class Camera;
	class RenderWindow;
	class Viewport;
	class SceneManager;
}

namespace AutoMatic
{
	/// This class handles the main camera functionality.
	/** 
	*** 
	*/
	class Camera
	{
	public:
		Camera(void)
			: _camera(0), _viewport(0)
		{ }

		Camera(std::string const & name_, Ogre::SceneManager * sceneManager_, Ogre::RenderWindow * window_
			, float left_ = 0.0f, float top_ = 0.0f, float width_ = 1.0f, float height_ = 1.0f);

		void position(float x_, float y_, float z_);
		void target(float x_, float y_, float z_);

		Ogre::Camera * get(void){ return _camera; }

	private:
		friend class boost::serialization::access;

		Ogre::Camera * _camera;
		Ogre::Viewport * _viewport;

		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Camera, "AutoMaticCamera")

#endif //_AM_Camera_hpp_