#include <cassert>
#include "Infinity_Piece.hpp"

namespace Infinity
{
	Piece::Piece(char const id_, char const rotation_, Value const left_, Value const right_, Value const top_, Value const bottom_) 
		: _id(id_), _rotation(rotation_)
	{
		_sides[Left] = left_; _sides[Right] = right_; 
		_sides[Top]  = top_; _sides[Bottom] = bottom_;
	}

	bool Piece::matches(Piece const * piece_) const
	{
		return (_sides[Left] == piece_->_sides[Left] &&
						_sides[Right] == piece_->_sides[Right] &&
						_sides[Top] == piece_->_sides[Top] &&
						_sides[Bottom] == piece_->_sides[Bottom] );
	}

	Value const Piece::side(Side const side_) const { return _sides[side_]; }

	Piece * Piece::rotateCW(void) const
	{
		assert(_rotation < 0x08);
		return new Piece(_id, _rotation << 1, side(Bottom), side(Top), side(Left), side(Right));
	}
}
