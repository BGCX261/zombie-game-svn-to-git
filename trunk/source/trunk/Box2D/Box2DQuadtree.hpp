#ifndef _Box2DQuadtree_hpp_
#define _Box2DQuadtree_hpp_

#include <set>
#include "AM_Quadtree.hpp"

struct Body;

namespace AutoMatic
{
	class Box2DQuadtreeNode : public QuadtreeNode
	{
	public:
		Box2DQuadtreeNode(Box2DQuadtreeNode * parent_, int x_, int y_, int width_, int minWidth_)
			: QuadtreeNode(parent_, x_, y_, width_, minWidth_)
		{
			_bodies.clear();
		}

		void add(Body * body_)
		{ 
			Box2DQuadtreeNode * currentNode = this;
			while(currentNode)
			{
				currentNode->_bodies.insert(body_);
				currentNode = static_cast<Box2DQuadtreeNode *>(currentNode->_parent);
			}
		}

		void rem(Body * body_)
		{ 
			Box2DQuadtreeNode * currentNode = this;
			while(currentNode)
			{
				currentNode->_bodies.erase(body_);
				currentNode = static_cast<Box2DQuadtreeNode *>(currentNode->_parent);
			}
		}

		std::set<Body *> & bodies(void){ return _bodies; }

		void clear(void)
		{ 
			_bodies.clear(); 
			QuadtreeNode ** c = children(false);
			if(c)
			{
				for (int i = 0; i < 4; ++i)
				{
					static_cast<Box2DQuadtreeNode *>(c[i])->clear();
				}
			}
		}

	protected:
		virtual QuadtreeNode * newNode(int const x_, int const y_, int const width_, int const minWidth_)
		{ 
			return new Box2DQuadtreeNode(this, x_, y_, width_, minWidth_); 
		}

		std::set<Body *> _bodies;
	};

	class Box2DQuadtree : public Quadtree
	{
	public:
		Box2DQuadtree(int width_, int minWidth_)
			: Quadtree(width_, minWidth_)
		{}

		void clear(void){ static_cast<Box2DQuadtreeNode *>(root())->clear(); }

	protected:
		virtual QuadtreeNode * newRoot(void)
		{ 
			return new Box2DQuadtreeNode(0, 50, -50, _width, _minWidth); 
		}
	};
}

#endif //_Box2DQuadtree_hpp_