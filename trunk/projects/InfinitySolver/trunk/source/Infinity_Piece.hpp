#ifndef _Infinity_Piece_hpp_
#define _Infinity_Piece_hpp_

#include <list>

namespace Infinity
{
	enum Side { Left = 0, Right, Top, Bottom, All_Sides };
	enum Value { Orange = 0, Pink, Purple, Blue, Edge, Empty, All_Values };

	class Piece
	{
	public:
		Piece(char const id_, char const rotation_, Value const left_, Value const right_, Value const top_, Value const bottom_);
		
		bool matches(Piece const * piece_) const;

		char const id(void) const { return _id; }
		char const rotation(void) const { return _rotation; }

		Value const side(Side const side_) const;

		Piece * rotateCW(void) const;

	private:
		char const _id, _rotation;
		Value _sides[All_Sides];

		Piece(Piece const & piece_);
		bool operator==(Piece const & piece_) const;
		Piece const & operator=(Piece const & piece_);
	};

	typedef std::list<Piece const *> Pieces;
}

#endif // _Infinity_Piece_hpp_