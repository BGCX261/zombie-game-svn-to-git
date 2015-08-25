#ifndef _AM_Building_hpp_
#define _AM_Building_hpp_

#include <vector>
#include "gpc/AM_GPC.hpp"

namespace AutoMatic
{
	class Building
	{
	public:
		Building(void) : _seed(0) {}
		Building(unsigned int seed_);

		void build(unsigned int seed_);

		std::vector<Polygon> & blueprints(void){ return _blueprints; }

	private:
		unsigned int _seed;

		std::vector<Polygon> _blueprints;
	};
}

#endif //_AM_Building_hpp_
