#ifndef _AM_CityQuadtree_hpp_
#define _AM_CityQuadtree_hpp_

#include "AM_Quadtree.hpp"

namespace AutoMatic
{
	class CityQuadtreeNode : public QuadtreeNode
	{
	public:
		CityQuadtreeNode(CityQuadtreeNode * parent_, int const x_, int const y_, int const w_);

	protected:
		virtual QuadtreeNode * newNode(int const x_, int const y_, int const width_);
	};

	class CityQuadtree : public Quadtree
	{
	public:
		CityQuadtree(int const width_, int const minWidth_);

	protected:
		virtual QuadtreeNode * newRoot(void);
	};
}

#endif //_AM_CityQuadtree_hpp_