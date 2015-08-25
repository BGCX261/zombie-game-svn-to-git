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

#include <fstream>
#include <string>
#include <iterator>

#include <gm/gmMachine.h>

#include "AM_Scripting.hpp"

namespace AutoMatic
{
	int LoadScript( gmMachine &machine_, char const *filename_, bool execute_ )
	{
		std::ifstream file(filename_);
		if (!file){	return GM_EXCEPTION; }

		std::string fileString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		
		return machine_.ExecuteString(fileString.c_str(), 0, execute_);
	}

	void HandleErrors( gmMachine & machine_ )
	{
		gmLog &log = machine_.GetLog();
		// Get the first error
		bool firstError = true;
		const char *err = log.GetEntry( firstError );
		while ( err )
		{
			// std::cout << "Compilation error: -" << err << std::endl;
			err = log.GetEntry( firstError );
		}
	}
}
