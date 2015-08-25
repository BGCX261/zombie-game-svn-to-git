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

#ifndef _AM_Serialisation_hpp_
#define _AM_Serialisation_hpp_

#include <string>
#include <fstream>

#include "AM_SerialisationIncludes.hpp"

namespace AutoMatic
{
	/// 
	/** Used to encapsulate the boost::serialization system.
	*** This will save an object (that can be serialised) to a file.
	*/
	class Serialise
	{
	public:
		Serialise(std::string const & filename_)
			: _outFile(filename_.c_str()), _outputArchive(0)
		{
			if(!_outFile.is_open())
			{
				throw std::runtime_error("Could not open " + filename_ + " for serialisation.");
			}

			_outputArchive = new boost::archive::text_oarchive(_outFile);
		}

		~Serialise(void)
		{
			delete _outputArchive;
		}

		template<class Out>
		bool out(Out const & out_)
		{
			(*_outputArchive) << out_;
			return true;
		}

	private:
		std::ofstream _outFile;
		boost::archive::text_oarchive * _outputArchive;
	};

	/// 
	/** Used to encapsulate the boost::serialization system.
	***	This will load an object (that can be serialised) from a file.
	*/
	class Deserialise
	{
	public:
		Deserialise(std::string const & filename_)
			: _inputFile(filename_.c_str()), _inputArchive(0)
		{
			if(!_inputFile.is_open())
			{
				throw std::runtime_error("Could not open " + filename_ + " for serialisation.");
			}

			_inputArchive = new boost::archive::text_iarchive(_inputFile);
		}

		~Deserialise(void)
		{
			delete _inputArchive;
		}

		template<class In>
		bool in(In & in_)
		{
			(*_inputArchive) >> in_;
			return true;
		}

	private:
		std::ifstream _inputFile;
		boost::archive::text_iarchive * _inputArchive;
	};

	void RunSerialisationTest(void);
}

#endif //_AM_Serialisation_hpp_
