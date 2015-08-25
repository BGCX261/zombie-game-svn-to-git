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

#include <cstdlib>
#include <cassert>
#include <ctime>

#include <boost/random.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include "AM_Random.hpp"

namespace AutoMatic
{
	/*
	double RandomD(double dMin, double dMax)
	{
		double dInterval = dMax - dMin;
		double d = dInterval * Random();
		return dMin + MIN(d, dInterval);
	}
	unsigned int RandomI(unsigned int nMin, unsigned int nMax)
	{
		unsigned int nInterval = nMax - nMin;
		unsigned int i = (unsigned int)((nInterval+1.0) * Random());
		return nMin + MIN(i, nInterval);
	}
	*/

	Random::Ptr Random::_singleton;

	class Random_Implementation : public Random
	{
	public:
		Random_Implementation(void)
		{ 
			_seed = time(0);
			srand(_seed); 
		}

		virtual float randomF(float min_, float max_)
		{
			int val = static_cast<int>((max_ - min_) * 1000);
			return min_ + static_cast<float>(rand() % val) / 1000.0f;
		}

		virtual int randomI(int min_, int max_)
		{
			int val = (max_ - min_) + 1;
			return min_ + (rand() % val);
		}

		unsigned int seed(unsigned int seed_)
		{
			unsigned int t = _seed;
			srand(seed_);
			_seed = seed_;
			return t;
		}
	};
	
	class BoostRandom_Implementation : public Random
	{
	public:
		BoostRandom_Implementation(void)
		{
			assert(false);
			_seed = 0;
		}
		virtual float randomF(float min_, float max_)
		{
			boost::mt19937 rnd;
			boost::uniform_real<> range(min_,max_);
			boost::variate_generator<boost::mt19937&, boost::uniform_real<> > random(rnd, range);
			return random();
		}

		virtual int randomI(int min_, int max_)
		{
			boost::mt19937 rnd;
			boost::uniform_int<> range(min_,max_);
			boost::variate_generator<boost::mt19937&, boost::uniform_int<> > random(rnd, range);
			return random();
		}

		unsigned int seed(unsigned int seed_)
		{
			assert(false);
			return 0;
		}
	};

	Random::Random(void)
	{
	}
	
	Random::Ptr Random::Get(void)
	{
		if(!_singleton){ _singleton.reset(new Random_Implementation); }
		return _singleton;
	}

	void RunRandomSeedAndGenerationTest(void)
	{
		for (int i = 0; i < 10; ++i)
		{
			int a,b,c,d,e;
			Random::Get()->seed(i);
			a = Random::Get()->randomI(10, 20);
			b = Random::Get()->randomI(10, 20);
			c = Random::Get()->randomI(10, 20);
			d = Random::Get()->randomI(10, 20);
			e = Random::Get()->randomI(10, 20);

			Random::Get()->seed(i);

			assert(Random::Get()->randomI(10, 20) == a);
			assert(Random::Get()->randomI(10, 20) == b);
			assert(Random::Get()->randomI(10, 20) == c);
			assert(Random::Get()->randomI(10, 20) == d);
			assert(Random::Get()->randomI(10, 20) == e);
		}
	}
}
