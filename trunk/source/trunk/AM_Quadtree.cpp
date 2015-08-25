#include <cassert>

#include "AM_Quadtree.hpp"

namespace AutoMatic
{
	bool IsPointInPolygon(float const xp_, float const yp_, std::vector< std::pair<Vector2, Vector2> > const & lines_)
	{
		int crossings = 0;
		for (std::vector< std::pair<Vector2, Vector2> >::const_iterator i = lines_.begin(); 
			i != lines_.end(); ++i)
		{
			if(DoesRayCrossLine(xp_, yp_, (*i))){ crossings++; }
		}

		return (crossings % 2) != 0;
	}

	/// Does a ray cast straight down from (x, y), cross the line [(x1, y1),(x2, y2)]?
	/** \param xp_ X component of the point to cast the ray from.
	*** \param yp_ Y component of the point to cast the ray from.
	*/
	bool DoesRayCrossLine(float const xp_, float const yp_, std::pair<Vector2, Vector2> const & line_)
	{
		if(line_.first.x == line_.second.x){ return xp_ == line_.first.x; }

		if(((line_.first.x < xp_ && xp_ < line_.second.x ) || 
			(line_.first.x > xp_ && xp_ > line_.second.x)) )
		{ // Is within line...
			float const t = (xp_ - line_.second.x) / (line_.first.x - line_.second.x);
			float const c = t*line_.first.y + (1.0f-t)*line_.second.y;
			return (c <= yp_);
		}
		else
		{
			return false;
		}
	}

//****************************************************************************//

	QuadtreeNode::QuadtreeNode(QuadtreeNode * parent_, int const x_, int const y_, int const w_, int const minWidth_)
		: _parent(parent_), _x(x_), _y(y_), _w(w_), _minWidth(minWidth_)
	{	
		for (int i=0;i<4;++i){ _children[i] = 0; }
	}

	QuadtreeNode::~QuadtreeNode(void)
	{
		for (int i=0;i<4;++i){ delete _children[i]; }
	}

	QuadtreeNode * * QuadtreeNode::children(bool create_)
	{
		if(_minWidth == _w){ return 0; } // This is a leaf node.
		if(!_children[0])
		{ 
			if(create_){ createChildren();  }
			else { return 0; }
		} // Create the children if needed.

		return _children;
	}

	int QuadtreeNode::width(void) const { return _w; }
	int QuadtreeNode::x(void) const { return _x; }
	int QuadtreeNode::y(void) const { return _y; }

	bool QuadtreeNode::inBounds(int const x_, int const y_)
	{
		return !((x_ > _x + _w) || (x_ < _x - _w) || (y_ > _y + _w) || (y_ < _y - _w));
	}

	bool QuadtreeNode::intersects(int const minX_, int const minY_, int const maxX_, int const maxY_)
	{
		return !(( minX_ > _x + _w ) ||	( maxX_ < _x - _w ) || ( minY_ > _y + _w ) || ( maxY_ < _w - _w ));
	}

	bool QuadtreeNode::contains(int const minX_, int const minY_, int const maxX_, int const maxY_)
	{
		return (minX_ >= _x - _w && minY_ >= _y - _w && maxX_ <= _x + _w && maxY_ <= _y + _w );
	}

	bool QuadtreeNode::contained(int const minX_, int const minY_, int const maxX_, int const maxY_)
	{
		return (minX_ <= _x - _w && minY_ <= _y - _w && maxX_ >= _x + _w && maxY_ >= _y + _w );
	}

	bool QuadtreeNode::contained(std::vector< std::pair<Vector2, Vector2> > const & lines_)
	{
		return (IsPointInPolygon(_x - _w, _y - _w, lines_)  &&
						IsPointInPolygon(_x + _w, _y - _w, lines_)  &&
						IsPointInPolygon(_x + _w, _y + _w, lines_)  &&
						IsPointInPolygon(_x - _w, _y + _w, lines_) );
	}

	QuadtreeNode * QuadtreeNode::container(int const minX_, int const minY_, int const maxX_, int const maxY_)
	{
		QuadtreeNode * currentNode = this;
		while (!currentNode->contains(minX_, minY_, maxX_, maxY_))
		{ // Find the bounding parent node
			QuadtreeNode * p = currentNode->_parent;
			if(!p){ return currentNode; } // Parent not found, default to last found.
			else { currentNode = p; }
			assert(currentNode);
		}

		QuadtreeNode ** c = 0;
		while ( (c = currentNode->children(true)) )
		{
			bool found = false;
			for (int i = 0; i < 4; ++i)
			{
				if(c[i]->contains(minX_, minY_, maxX_, maxY_)){ currentNode = c[i]; found = true; break; }
			}

			assert(currentNode);
			if(!found){ break; }
		}

		return currentNode;
	}

	/// Find a node from a point, with this node as a starting point.
	QuadtreeNode * QuadtreeNode::find(int const x_, int const y_, int const minWidth_)
	{
		if(_minWidth > minWidth_){ return 0; } // Can't search below a minimum width of 1.

		QuadtreeNode * currentNode = this;
		// While the point is out of this nodes bounds, head up the tree.
		while (!currentNode->inBounds(x_,y_))
		{
			if(!currentNode->_parent){ return 0; } // The point is completely out of bounds.
			currentNode = currentNode->_parent;
		}

		// While the encompassing node is not a leaf, head down the tree.
		// Note that currentNode should NEVER be null.
		while (currentNode && currentNode->width() > minWidth_ )
		{ // While the width of the node is greater than the desired minimum width...
			currentNode = currentNode->child(x_ ,y_);
		}

		assert(currentNode);
		return currentNode;
	}

	/// Find the first child that encompasses the \a x_ , \a y_ point.
	QuadtreeNode * QuadtreeNode::child(int const x_, int const y_)
	{
		if(_minWidth == _w && x_ == _x && y_ == _y){ return this; }

		QuadtreeNode * * c = children(true);
		if(!c){ assert(false); return 0; }

		// This algorithm favours the p[X|Y], because it tests >= or <
		if(x_ >= _x)
		{ 
			if(y_ >= _y){ return c[pXpY]; }
			else { return c[pXnY]; }
		}
		else // (x_ < _x)
		{
			if(y_ >= _y){ return c[nXpY]; }
			else { return c[nXnY]; }
		}
	}

	QuadtreeNode * QuadtreeNode::child(ChildId const id_)
	{
		if(_minWidth == _w){ return 0; }
		
		QuadtreeNode * * c = children(true);
		if(!c){ assert(false); return 0; }
		return c[id_];
	}

	bool QuadtreeNode::intersections(QuadtreeNode * * intersections_, 
		int const minX_, int const minY_, int const maxX_, int const maxY_)
	{
		if(_minWidth == _w){ return false; }
		QuadtreeNode * * c = children(true);
		if(!c){ assert(false); return false; }

		bool hasIntersection = false;
		for (int i = 0; i < 4; ++i)
		{
			if(c[i]->intersects(minX_, minY_, maxX_, maxY_))
			{ 
				intersections_[i] = c[i]; 
				hasIntersection = true;
			}
			else { intersections_[i] = 0; }
		}

		return hasIntersection;
	}

	void QuadtreeNode::createChildren(void)
	{
		if(_w <= _minWidth){ return; } // This is a leaf node.
		int w = _w / 2;

		_children[pXpY] = newNode(_x + w, _y + w, w, _minWidth);
		_children[pXnY] = newNode(_x + w, _y - w, w, _minWidth);
		_children[nXnY] = newNode(_x - w, _y - w, w, _minWidth);
		_children[nXpY] = newNode(_x - w, _y + w, w, _minWidth);
	}

	QuadtreeNode * QuadtreeNode::newNode(int const x_, int const y_, int const width_, int const minWidth_)
	{
		return new QuadtreeNode(this, x_, y_, width_, minWidth_);
	}

//****************************************************************************//

	Quadtree::Quadtree(int const width_, int const minWidth_)
		: _root(0), _width(width_), _minWidth(minWidth_)
	{

	}

	Quadtree::~Quadtree(void)
	{
		delete _root;
	}

	/// Obtain the leaf centred \a x_, \a y_.
	QuadtreeNode * Quadtree::find(int const x_, int const y_, int const minWidth_)
	{
		if(!_root){ _root = newRoot(); }
		return _root->find(x_, y_, minWidth_);
	}

	QuadtreeNode * Quadtree::root(void)
	{ 
		if(!_root){ _root = newRoot(); }
		return _root; 
	}

	QuadtreeNode * Quadtree::newRoot(void){ return new QuadtreeNode(0, 0, 0, _width, _minWidth); }

	void RunQuadtreeCreationAndNodeSelectionTest(void)
	{
		Quadtree testTree(256, 1);

		QuadtreeNode * testNode = 0;

		testNode = testTree.find(0, 0);
		assert(testNode && testNode->width() == 1 && testNode->x() == 1 && testNode->y() == 1);
		testNode = testTree.find(0, 0, 256);
		assert(testNode && testNode->width() == 256 && testNode->x() == 0 && testNode->y() == 0);
		assert(testNode == testTree.root());
		testNode = testTree.find(0, 0, 128);
		assert(testNode && testNode->width() == 128 && testNode->x() == 128 && testNode->y() == 128);
		testNode = testTree.find(0, 0, 64);
		assert(testNode && testNode->width() == 64 && testNode->x() == 64 && testNode->y() == 64);
		testNode = testTree.find(0, 0, 32);
		assert(testNode && testNode->width() == 32 && testNode->x() == 32 && testNode->y() == 32);
		testNode = testTree.find(0, 0, 16);
		assert(testNode && testNode->width() == 16 && testNode->x() == 16 && testNode->y() == 16);
		testNode = testTree.find(0, 0, 8);
		assert(testNode && testNode->width() == 8 && testNode->x() == 8 && testNode->y() == 8);
		testNode = testTree.find(0, 0, 2);
		assert(testNode && testNode->width() == 2 && testNode->x() == 2 && testNode->y() == 2);

		testNode = testTree.find(1,1);
		assert(testNode && testNode->width() == 1 && testNode->x() == 1 && testNode->y() == 1);

		testNode = testTree.find(-1,1);
		assert(testNode && testNode->width() == 1 && testNode->x() == -1 && testNode->y() == 1);

		testNode = testTree.find(-1,-1);
		assert(testNode && testNode->width() == 1 && testNode->x() == -1 && testNode->y() == -1);

		testNode = testTree.find(1,-1);
		assert(testNode && testNode->width() == 1 && testNode->x() == 1 && testNode->y() == -1);

		int minX = -113, maxX = 129, minY = -10, maxY = 145;
		QuadtreeNode * intersect1[4], * intersect2[4];
		
		assert(testTree.root()->intersections(intersect1, minX, minY, maxX, maxY));
		assert(intersect1[0] && intersect1[1] &&  intersect1[2] &&  intersect1[3]);
		
		assert(intersect1[0]->intersections(intersect2, minX, minY, maxX, maxY));
		assert(intersect2[0] && intersect2[1] &&  intersect2[2] &&  intersect2[3]);

		assert(intersect1[1]->intersections(intersect2, minX, minY, maxX, maxY));
		assert(intersect2[0] && !intersect2[1] &&  !intersect2[2] &&  intersect2[3]);

		assert(intersect1[2]->intersections(intersect2, minX, minY, maxX, maxY));
		assert(intersect2[0] && !intersect2[1] &&  !intersect2[2] &&  !intersect2[3]);

		assert(intersect1[3]->intersections(intersect2, minX, minY, maxX, maxY));
		assert(intersect2[0] && intersect2[1] &&  !intersect2[2] &&  !intersect2[3]);
	}
//****************************************************************************//
}
