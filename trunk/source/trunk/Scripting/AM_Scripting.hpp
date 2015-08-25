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

#ifndef _AM_Scripting_hpp_
#define _AM_Scripting_hpp_

// http://www.gamedev.net/reference/articles/article2296.asp

class gmMachine;

namespace AutoMatic
{
	/// 
	/** \param execute_ If true, the script will be executed immediately.
	*** \return If != 0, an error has occurred.
	*/
	int LoadScript( gmMachine &machine_, char const *filename_, bool execute_ );

	void HandleErrors( gmMachine & machine_ );
}

#endif //_AM_Scripting_hpp_