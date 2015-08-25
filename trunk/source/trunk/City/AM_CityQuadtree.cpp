#include "AM_CityQuadtree.hpp"

namespace AutoMatic
{
//****************************************************************************//

	CityQuadtreeNode::CityQuadtreeNode(CityQuadtreeNode * parent_, int const x_, int const y_, int const w_)
		: QuadtreeNode(parent_, x_, y_, w_) 
	{}

	QuadtreeNode * CityQuadtreeNode::newNode(int const x_, int const y_, int const width_)
	{
		return new CityQuadtreeNode(this, x_, y_, width_);
	}

//****************************************************************************//

	CityQuadtree::CityQuadtree(int const width_, int const minWidth_)
		: Quadtree(width_, minWidth_) 
	{}

	QuadtreeNode * CityQuadtree::newRoot(void)
	{
		return new CityQuadtreeNode(0,0,0,_width);
	}

//****************************************************************************//
}
