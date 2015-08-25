#include <cassert>
#include <iostream>
#include <algorithm>
#include "Infinity_Board.hpp"

namespace Infinity
{
	void Tile::countNonEmptyAndEdges(void)
	{ 
		_edgeFlags = 0x11; 
		for(int i = 0; i < All_Sides; ++i)
		{ 
			if(_sides[i] != Empty)
			{ 
				_edgeFlags |= ((_edgeFlags & 0xf0) << 1);
				if(_sides[i] == Edge){ _edgeFlags |= ((_edgeFlags & 0x0f) << 1); }
			} 
		} 
	}

	bool Tile::operator<(Tile const & tile_) const
	{
		// First, sort by non-empty sides and edges, to attempt faster culling of invalid searches.
		if(_edgeFlags > tile_._edgeFlags){ return true; }
		if(_edgeFlags < tile_._edgeFlags){ return false; }
		// Third, no special reason.
		return !(_x >= tile_._x || _y >= tile_._y);
	}

	bool Tile::operator==(Tile const & tile_) const
	{
		return _x == tile_._x && _y == tile_._y;
	}

	Board::Board(char const width_, char const height_, char const startX_, char const startY_)
		: _width(width_), _height(height_), _startX(startX_), _startY(startY_), _board(0)
	{
		createNewBoard();
		for(char i = 0; i < _width*_height; ++i){ _board[i] = 0; }
		addToOpenTiles(_startX, _startY);
	}

	Board::Board(Board const & board_)
		: _width(board_._width), _height(board_._height	), _startX(board_._startX), _startY(board_._startY), _board(0)
		, _openTiles(board_._openTiles)
	{
		createNewBoard();
		for(int i = 0; i < _width*_height; ++i){ _board[i] = board_._board[i]; }
	}

	void Board::createNewBoard(void)
	{
		delete[] _board;
		assert(_width > 0); assert(_height > 0);
		_board = new Piece const * [_width * _height];		
	}

	Board::~Board(void)
	{
		delete[] _board;
	}

	Piece const * Board::getPiece(char const x_, char const y_) const
	{
		return _board[y_*_width + x_];
	}

	void Board::setPiece(Piece const * piece_, char const x_, char const y_)
	{
		assert(!isEdge(x_,y_));
		_board[y_*_width + x_] = piece_;
	}

	bool Board::isEdge(char const x_, char const y_) const
	{
		return (x_ < 0 || x_ >= _width || y_ < 0 || y_ >= _height);
	}

	void Board::place(Piece const * piece_, char const x_, char const y_)
	{
		setPiece(piece_, x_, y_);

		removeFromOpenTiles(Tile(x_,y_));

		addToOpenTiles(x_-1,y_);
		addToOpenTiles(x_+1,y_);
		addToOpenTiles(x_,y_-1);
		addToOpenTiles(x_,y_+1);
	}

	Tile const * Board::fit(Piece const * piece_) const
	{
		assert(_openTiles.size() > 0);

		Tile const & t = *_openTiles.begin();
		
		if(!fits(piece_, t)){ return 0;	}
		else { return &t; }
	}

	bool Board::fits(Piece const * piece_, Tile const & tile_) const
	{
		Value const l = tile_.side(Left), r = tile_.side(Right), t = tile_.side(Top), b = tile_.side(Bottom);
		return ((l == Empty || piece_->side(Left) == l) && 
						(r == Empty || piece_->side(Right) == r) &&
						(t == Empty || piece_->side(Top) == t ) &&
						(b == Empty || piece_->side(Bottom) == b));
	} 

	void Board::setTileBounds(Tile & tile_, char const x_, char const y_, Side const side_)
	{
		if(!isEdge(x_,y_))
		{
			Piece const * piece = getPiece(x_,y_);
			if(0 == piece)
			{
				tile_.side(side_, Empty);
			}
			else
			{
				switch(side_)
				{
				case Left: { tile_.side(side_, piece->side(Right)); } break;
				case Right: { tile_.side(side_, piece->side(Left)); } break;
				case Top: { tile_.side(side_, piece->side(Bottom)); } break;
				case Bottom: { tile_.side(side_, piece->side(Top)); } break;
				}
			}
		}
		else
		{
			tile_.side(side_, Edge);
		}
	}

	void Board::addToOpenTiles(char const x_, char const y_)
	{
		if(!isEdge(x_,y_))
		{
			Piece const * piece = getPiece(x_,y_);
			if(0 == piece)
			{
				Tile openTile(x_,y_);
				
				setTileBounds(openTile, x_-1, y_, Left);
				setTileBounds(openTile, x_+1, y_, Right);
				setTileBounds(openTile, x_, y_+1, Top);
				setTileBounds(openTile, x_, y_-1, Bottom);

				removeFromOpenTiles(openTile);

				openTile.countNonEmptyAndEdges();

				_openTiles.insert(openTile);
			}
		}
	}

	void Board::removeFromOpenTiles(Tile const & tile_)
	{
		Tiles::iterator i = std::find(_openTiles.begin(), _openTiles.end(), tile_);
		if(i != _openTiles.end()){ _openTiles.erase(i); }
	}

	void Board::debugOutput(void)
	{
		for (char i = 0; i < _height; ++i)
		{
			for (char j = 0; j < _width; ++j)
			{
				std::cout << (_board[i * _width + j] ? _board[i * _width + j]->id() : -1) << " ";
			}
			std::cout << std::endl;
		}
	}
}
