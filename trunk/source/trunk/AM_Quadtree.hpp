#ifndef _AM_QuadTree_hpp_
#define _AM_QuadTree_hpp_

#include <vector>

class Vector2
{
public:
	float x, y;
};

namespace AutoMatic
{
//****************************************************************************//
	bool DoesRayCrossLine(float const xp_, float const yp_, std::pair<Vector2, Vector2> const & line_);
	bool IsPointInPolygon(float const xp_, float const yp_, std::vector< std::pair<Vector2, Vector2> > const & lines_);

	/// 
	/** Defines a quadtree node.  A node is either a leaf or a parent node.
	*** A leaf has no children and has a width of 1.
	*/
	class QuadtreeNode
	{
	public:
		enum ChildId { pXpY, pXnY, nXnY, nXpY };

		/// 
		/** \param w_ A width of 1 means this is a leaf node.
		***
		*/
		QuadtreeNode(QuadtreeNode * parent_, int const x_, int const y_, int const w_, int const minWidth_ = 1);

		/// 
		/** Destroy the quadtree, including all children.
		***
		*/
		virtual ~QuadtreeNode(void);

		/// Get the quadtree's children.  
		/** \param create_ If true, create the children if they aren't already, if this node is not a leaf.
		*/
		QuadtreeNode * * children(bool create_);

		/// Get the width of the quad-tree node.
		int width(void) const;
		/// Get the x component of the origin.
		int x(void) const;
		/// Get the y component of the origin.
		int y(void) const;

		/// Is the point \a x_ , \a y_ in the bounds of this node?
		bool inBounds(int const x_, int const y_);

		/// Does the box intersect this node?
		bool intersects(int const minX_, int const minY_, int const maxX_, int const maxY_);

		/// Is the box wholly contained within this node?
		bool contains(int const minX_, int const minY_, int const maxX_, int const maxY_);

		/// Is this node wholly contained within the box?
		bool contained(int const minX_, int const minY_, int const maxX_, int const maxY_);

		/// Is this node wholly contained within the polygon defined by the lines?
		bool contained(std::vector< std::pair<Vector2, Vector2> > const & lines_);

		/// Find the node that contains the box, starting from this node
		/** This will move up the hierarchy, if needed.
			* If the box doesn't fit into the root, the root is still returned.
			* This will create children down to the required level.
			* \return The returned value will NEVER be null.
		*/
		QuadtreeNode * container(int const minX_, int const minY_, int const maxX_, int const maxY_);

		/// Find a node from a point, with this node as a starting point.
		/** \param width_ The search will stop when a QuadtreeNode has a width <= to this.
		*/
		QuadtreeNode * find(int const x_, int const y_, int const minWidth_ = 1);

		/// Find the first child that encompasses the \a x_ , \a y_ point.
		QuadtreeNode * child(int const x_, int const y_);

		QuadtreeNode * child(ChildId const id_);

		/// Get the child nodes that the box intersects.
		/** \return true if any child intersects the box.
		*/
		bool intersections(QuadtreeNode * * intersections_, int const minX_, int const minY_, int const maxX_, int const maxY_);

		QuadtreeNode * parent(void){ return _parent; }

	protected:
		/// Created children of this quadtree node.  Does nothing if this is a leaf
		void createChildren(void);
		
		/// Create a new QuadtreeNode.  Override to provide a derived type of node.
		virtual QuadtreeNode * newNode(int const x_, int const y_, int const width_, int const minWidth_);

		QuadtreeNode * _parent; /**< The parent of this quadtree.  Null if this is the root. **/
		QuadtreeNode * _children[4]; /**< The children of this quadtree.  Null if this is a leaf. **/

		int _x, _y, _w, _minWidth; /**< Dimensions: (X,Y) centre and width. **/
	};

//****************************************************************************//

	/// 
	/** 
	***
	*/
	class Quadtree
	{
	public:
		/// Create a new tree of the desired \a width_.
		Quadtree(int const width_, int const minWidth_);

		/// Destroy the tree.
		virtual ~Quadtree(void);

		/// Obtain the leaf centred \a x_, \a y_.
		QuadtreeNode * find(int const x_, int const y_, int const minWidth_ = 1);

		/// Get the root node of this tree.
		QuadtreeNode * root(void);

	protected:
		/// Create a new root.  Override to provide a derived type of node.
		virtual QuadtreeNode * newRoot(void);

		int _width; /**< Store the width of the tree, for lazy creation of the root. **/
		int _minWidth;

	private:
		QuadtreeNode * _root; /**< The root of the QuadTree **/

	};

	void RunQuadtreeCreationAndNodeSelectionTest(void);
//****************************************************************************//
}

#endif //_AM_QuadTree_hpp_