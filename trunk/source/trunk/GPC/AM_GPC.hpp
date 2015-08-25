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

#ifndef _AM_GPC_hpp_
#define _AM_GPC_hpp_

#include <vector>

#include "gpc.hpp"

namespace AutoMatic
{
	class Polygon;
	/// Polygon creation class.
	/** Create Polygons manually, and by clipping polygons together with the 
	*** provided operators.
	*/
	class Vertices
	{
	public:
		struct Vertex
		{
			Vertex(void) : _x(0.0), _y(0.0) {}
			Vertex(double x_, double y_) : _x(x_), _y(y_) {}

			double _x, _y;
		};

		Vertices(void)
		{}

		Vertices(Vertices const & copy_)
		{
			_vertices = copy_._vertices;
		}

		Vertices const & operator=(Vertices const & copy_)
		{
			_vertices = copy_._vertices;
			return *this;
		}

		typedef std::vector<Vertex> VertexList;

		void add(double x_, double y_);
		
		VertexList const & getList(void) const { return _vertices; }
		/// Translate the polygon
		Vertices operator+(Vertex const & rhs_) const;

	protected:
		VertexList _vertices;
	};

	/// This class is used to calculate multiple polygon operations on a gpc_polygon, before assigning the vertices to a Polygon.
	/// I do this because I'm not sure that simply converting to a gpc_tristip and copying the vertices for every operation is the best idea.
	/// Instead, a copy-constructor for the Polygon does that job, after the operations are completed.
	class Polygon : public Vertices
	{
	public:
		Polygon(void);
		~Polygon(void);
		/// Intersection
		void operator&=(Vertices const & rhs_);
		/// X-or
		void operator^=(Vertices const & rhs_);
		/// Difference
		void operator-=(Vertices const & rhs_);
		/// Union
		void operator|=(Vertices const & rhs_);

		gpc_polygon & get(void){ return _polygon; }

		int size(void);

		Vertices::Vertex getVertex(int index_);

	private:
		gpc_polygon _polygon;
	};
}

#endif //_AM_GPC_hpp_