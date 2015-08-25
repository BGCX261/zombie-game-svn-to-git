#include <cassert>
#include "AM_Random.hpp"

#include "AM_Building.hpp"

namespace AutoMatic
{
//****************************************************************************//

	Building::Building(unsigned int seed_)
		: _seed(seed_)
	{
		build(_seed);
	}

	void Building::build(unsigned int seed_)
	{
		_seed = seed_;
		_blueprints.clear();

		// Assuming 32bit unsigned int...
		assert(4 == sizeof(_seed));

		Random::Get()->seed(_seed);

		// 1. Get Levels (number of floors) from seed.
		unsigned int levels = Random::Get()->randomI(1, 10);
		_blueprints.reserve(levels);

		// The blueprint for the current floor plan, carried on to the next floor plan.
		Polygon blueprint;
		int randX = 1, randZ = 1;

		// 2. For each floor plan:
		for(unsigned int floorPlanNum = 0; floorPlanNum < levels; ++floorPlanNum)
		{
			// 2.1 Generate random number of iterations.
			int iterations = Random::Get()->randomI(1, 3);
			// 2.2 For each iteration:
			for (int iterationNum = 0; iterationNum < iterations; ++iterationNum)
			{
				// 2.2.1 Generate a random polygon, R (centred about the origin).
				Vertices randomPoly;
				randX = Random::Get()->randomI(1, 3);
				randZ = Random::Get()->randomI(1, 3);
				randomPoly.add(randX * -1.0, randZ * -1.0);

				randX = Random::Get()->randomI(1, 3);
				randZ = Random::Get()->randomI(1, 3);
				randomPoly.add(randX * 1.0, randZ * -1.0);

				randX = Random::Get()->randomI(1, 3);
				randZ = Random::Get()->randomI(1, 3);
				randomPoly.add(randX * 1.0, randZ * 1.0);

				randX = Random::Get()->randomI(1, 3);
				randZ = Random::Get()->randomI(1, 3);
				randomPoly.add(randX * -1.0, randZ * 1.0);

				// 2.2.2 Rotate R about a random angle.
				// TODO

				// 2.2.3 Translate the centre of R a random vertex of the blueprint.
				int bsize = blueprint.size();
				if(bsize > 0)
				{ 
					randomPoly = randomPoly + blueprint.getVertex( Random::Get()->randomI(0, bsize - 1) );
				}

				// 2.2.4 Set the blueprint to the union of R and the blueprint.
				blueprint |= randomPoly;
			}			
			// 2.2 Add the blueprint to the vector of floorPlans.
			_blueprints.push_back(blueprint);
		}
	}

//****************************************************************************//
}
