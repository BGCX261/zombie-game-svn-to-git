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

#ifndef _AM_City_hpp_
#define _AM_City_hpp_

#include "AM_CityQuadtree.hpp"

namespace AutoMatic
{
	class City
	{
	public:
		City(int seed_, int width_, int minWidth_);

	private:
		int _seed;
		CityQuadtree _quadtree;

		unsigned int makeHash(int key_);

		unsigned int getCellSeed(int x_, int z_);
	};
}

#endif //_AM_City_hpp_