#ifndef _Infinity_Board_hpp_
#define _Infinity_Board_hpp_
#include <set>
#include <map>
#include "Infinity_Piece.hpp"

namespace Infinity
{
	Infinity::Piece *  AddPiece(Infinity::Pieces & pieces_, PieceId id_,
		Infinity::Value l_, Infinity::Value r_, Infinity::Value t_, Infinity::Value b_);

	void PiecesAnalyse(Pieces * pieces_, Value colours_);

	typedef unsigned char EdgeFlags;

	EdgeFlags CountNonEmptyAndEdges(Value * sides_);

	class SortPieces
	{
	public:
		SortPieces(Pieces * pieces_, int seed_);

		bool operator()(Piece const * lhs_, Piece const * rhs_) const;

		void analyseValues(Pieces * pieces_);

		bool _betterIfMoreEdges;
		bool _betterIfCommon;

	private:
		mutable std::map<Value, int> _countValues; /**< How often each colour occurs. **/
		mutable std::map<int, int> _sameness; /**< How often pieces have [1,2,3,4] colours. **/
	};

	class Tile
	{
	public:
		Tile(CooOrd const x_, CooOrd const y_)
			: _x(x_), _y(y_), _edgeFlags(0x11)
		{}

		bool operator<(Tile const & tile_) const;

		bool operator==(Tile const & tile_) const;

		CooOrd const x(void) const { return _x; }
		CooOrd const y(void) const { return _y; }

		Value const side(Side const side_) const { return _sides[side_]; }
		void side(Side const side_, Value const value_){ _sides[side_] = value_; }

		Value const * sides(void) const { return _sides; }

		void edgeFlags(EdgeFlags const edgeFlags_){ _edgeFlags = edgeFlags_; }

	private:
		CooOrd const _x, _y;
		EdgeFlags _edgeFlags;
		
		Value _sides[All_Sides];

		Tile const & operator=(Tile const & tile_);
	};

	typedef std::set<Tile const> Tiles;

	class Board
	{
	public:

		/// Generate a random board.
		static Infinity::Pieces * GeneratePieces(unsigned int seed_, CooOrd const width_, CooOrd const height_, Value const colours_);

		Board(CooOrd const width_, CooOrd const height_, CooOrd const startX_ = 0, CooOrd const startY_ = 0);
		Board(Board const & board_);

		Board(std::ifstream & file_, Pieces const & pieces_);

		~Board(void);

		/// 
		/** Place a piece in the specified position.
		*** \todo Optimise.
		*/
		void place(Piece const * piece_, CooOrd const x_, CooOrd const y_);
		
		/// 
		/** Check if the piece fits to the first available open tile.
		*** If so, add it to the fitted pieces.
			* If it already matches (in side values) a piece already on
			* the fittedPieces_ list, ignore it.
			* \todo Optimise.
		*/
		Tile const * fit(Piece const * piece_) const;

		bool fits(Piece const * piece_, Tile const & tile_) const;

		Piece const * getPiece(CooOrd const x_, CooOrd const y_) const;
		void setPiece(Piece const * piece_, CooOrd const x_, CooOrd const y_);
		bool isEdge(CooOrd const x_, CooOrd const y_) const;

		void debugOutput(void) const;

		Tiles const & openTiles(void) const { return _openTiles; }

		CooOrd const startX(void) const { return _startX; }
		CooOrd const startY(void) const { return _startY; }
		CooOrd const width(void) const { return _width; }
		CooOrd const height(void) const { return _height; }

		Piece const * * board(void) const { return _board; }

		bool verify(void) const;
		void analyse(Value colours_) const;

		void save(std::ofstream & file_) const;

	private:
		CooOrd _width, _height, _startX, _startY;
		Tiles _openTiles;

		Piece const * * _board;

		void createNewBoard(void);
		void addToOpenTiles(CooOrd const x_, CooOrd const y_);
		void removeFromOpenTiles(Tile const & tile_);
		void setTileBounds(Tile & tile_);
		void setTileSide(Tile & tile_, CooOrd const x_, CooOrd const y_, Side const side_);

		Board const & operator=(Board const & board_);
	};
}

#endif // _Infinity_Board_hpp_