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

#include "AM_City.hpp"

namespace	AutoMatic
{
//****************************************************************************//

	City::City(int seed_, int width_, int minWidth_)
		: _seed(seed_), _quadtree(width_, minWidth_)
	{}

	unsigned int City::makeHash(int key_)
	{
		key_ += ~(key_ << 15);
		key_ ^= (key_ >> 10);
		key_ += (key_ << 3);
		key_ ^= (key_ >> 6);
		key_ += ~(key_ << 11);
		key_ ^= (key_ >> 16);
		return key_;
	}

	unsigned int City::getCellSeed(int x_, int z_)
	{
		return makeHash(x_^makeHash(z_^_seed));
	}

//****************************************************************************//
}