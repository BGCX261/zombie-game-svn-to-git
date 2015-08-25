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

#include <cassert>
#include "AM_GPC.hpp"

namespace AutoMatic
{
//****************************************************************************//
	void Vertices::add(double x_, double y_)
	{
		Vertex temp; temp._x = x_; temp._y = y_;
		_vertices.push_back(temp);
	}

	void Generate(Vertices::VertexList const & vertices_, gpc_polygon & polygon_)
	{
		polygon_.contour = 0;
		polygon_.hole = 0;
		polygon_.num_contours = 0;

		gpc_add_contour_parts(&polygon_, vertices_.size(), 
			reinterpret_cast<gpc_vertex const *>(&(vertices_[0])), 0);
	}

	Vertices Vertices::operator+(Vertices::Vertex const & rhs_) const
	{
		Vertices ret;
		ret._vertices.reserve(_vertices.size());
		for (std::vector<Vertices::Vertex>::const_iterator i = _vertices.begin();
			i != _vertices.end(); ++i)
		{
			ret._vertices.push_back(Vertex((*i)._x + rhs_._x, (*i)._y + rhs_._y));
		}

		return ret;
	}

	void Clip(gpc_op op_, gpc_polygon & lhs_, Vertices const & rhs_)
	{
		gpc_polygon polygonrhs;

		if(rhs_.getList().size() == 0)
		{ 
			return; 
		}
		else if(lhs_.num_contours == 0)
		{ 
			gpc_free_polygon(&lhs_);
			Generate(rhs_.getList(), lhs_);
		}
		else
		{
			gpc_polygon ret;
			Generate(rhs_.getList(), polygonrhs);

			gpc_polygon_clip(op_, &lhs_, &polygonrhs, &ret);

			gpc_free_polygon(&polygonrhs);
			gpc_free_polygon(&lhs_);

			lhs_ = ret;
		}
	}

//****************************************************************************//

	Polygon::Polygon(void)
	{
		gpc_tristrip tristrip;
		gpc_polygon_to_tristrip(&_polygon, &tristrip);

		for (int i = 0; i < tristrip.num_strips; ++i)
		{ // TODO: Is this correct?
			for (int j = 0; j < tristrip.strip[i].num_vertices; ++j)
			{
				add(tristrip.strip[i].vertex[j].x,	tristrip.strip[i].vertex[j].y);
			}			
		}

		gpc_free_tristrip(&tristrip);
	}

	Polygon::~Polygon(void)
	{
		gpc_free_polygon(&_polygon);
	}

	/// Intersection
	void Polygon::operator&=(Vertices const & rhs_)
	{
		Clip(GPC_INT, _polygon, rhs_);
	}

	/// X-or
	void Polygon::operator^=(Vertices const & rhs_)
	{
		Clip(GPC_XOR, _polygon, rhs_);
	}

	/// Difference
	void Polygon::operator-=(Vertices const & rhs_)
	{
		Clip(GPC_DIFF, _polygon, rhs_);
	}

	/// Union
	void Polygon::operator|=(Vertices const & rhs_)
	{
		Clip(GPC_UNION, _polygon, rhs_);
	}

	int Polygon::size(void)
	{
		int vertexCount = 0;
		for (int i = 0; i < _polygon.num_contours; ++i)
		{
			vertexCount += _polygon.contour[i].num_vertices;
		}

		return vertexCount;
	}

	Vertices::Vertex Polygon::getVertex(int index_)
	{
		Vertices::Vertex vertex;
		int vertexCount = 0;
		for (int i = 0; i < _polygon.num_contours; ++i)
		{
			if(index_ < _polygon.contour[i].num_vertices)
			{
				gpc_vertex v = _polygon.contour[i].vertex[index_];
				vertex._x = v.x; vertex._y = v.y;
			}
			else
			{
				index_ -= _polygon.contour[i].num_vertices;
			}
		}

		return vertex;
	}
}
