#ifndef _Infinity_Board_hpp_
#define _Infinity_Board_hpp_
#include <set>
#include "Infinity_Piece.hpp"

namespace Infinity
{
	class Tile
	{
	public:
		Tile(char const x_, char const y_)
			: _x(x_), _y(y_), _edgeFlags(0x11)
		{}

		void countNonEmptyAndEdges(void);

		bool operator<(Tile const & tile_) const;

		bool operator==(Tile const & tile_) const;

		char const x(void) const { return _x; }
		char const y(void) const { return _y; }

		Value const side(Side const side_) const { return _sides[side_]; }
		void side(Side const side_, Value const value_){ _sides[side_] = value_; }

	private:
		char const _x, _y;
		unsigned char _edgeFlags;
		
		Value _sides[All_Sides];

		Tile const & operator=(Tile const & tile_);
	};

	typedef std::set<Tile const> Tiles;

	class Board
	{
	public:

		Board(char const width_, char const height_, char const startX_ = 0, char const startY_ = 0);
		Board(Board const & board_);

		~Board(void);

		/// 
		/** Place a piece in the specified position.
		*** \todo Optimise.
		*/
		void place(Piece const * piece_, char const x_, char const y_);
		
		/// 
		/** Check if the piece fits to the first available open tile.
		*** If so, add it to the fitted pieces.
			* If it already matches (in side values) a piece already on
			* the fittedPieces_ list, ignore it.
			* \todo Optimise.
		*/
		Tile const * fit(Piece const * piece_) const;

		bool fits(Piece const * piece_, Tile const & tile_) const;

		Piece const * getPiece(char const x_, char const y_) const;
		void setPiece(Piece const * piece_, char const x_, char const y_);
		bool isEdge(char const x_, char const y_) const;

		void debugOutput(void);

		Tiles const & openTiles(void) const { return _openTiles; }

		char const startX(void) const { return _startX; }
		char const startY(void) const { return _startY; }
		char const width(void) const { return _width; }
		char const height(void) const { return _height; }

		Piece const * * board(void) const { return _board; }

	private:
		char const _width, _height, _startX, _startY;
		Tiles _openTiles;

		Piece const * * _board;

		void createNewBoard(void);
		void addToOpenTiles(char const x_, char const y_);
		void removeFromOpenTiles(Tile const & tile_);
		void setTileBounds(Tile & tile_, char const x_, char const y_, Side const side_);

		Board const & operator=(Board const & board_);
	};
}

#endif // _Infinity_Board_hpp_