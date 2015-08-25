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

#ifndef _AutoMatic_Random_h_
#define _AutoMatic_Random_h_

#include <boost/shared_ptr.hpp>

namespace AutoMatic
{
	class Random
	{
	public:
		typedef boost::shared_ptr<Random> Ptr;
	
		static Random::Ptr Get(void);

		virtual ~Random(void){}

		/// Generate a random float between a minimum and maximum value, inclusive.
		virtual float randomF(float min_, float max_) = 0;

		/// Generate a random integer between a minimum and maximum value, inclusive.
		virtual int randomI(int min_, int max_) = 0;

		/// Set the seed of the Random number generator.
		/** \return The previous seed value.
		*/
		virtual unsigned int seed(unsigned int seed_) = 0;

	protected:
		static Ptr _singleton;
		unsigned int _seed;

		Random(void);
	};

	void RunRandomSeedAndGenerationTest(void);
}

#endif //_AutoMatic_Random_h_
