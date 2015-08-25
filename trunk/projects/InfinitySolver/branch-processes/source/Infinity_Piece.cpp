#include <cassert>
#include "Infinity_Piece.hpp"

#include "AM_Memory.h"

namespace Infinity
{
	Value const Edge		= 0x00;
	Value const Empty		= 0x01;
	Value const All_Special_Colours = 0x02;
	Value const Orange	= 0x02;
	Value const Pink		= 0x03; 
	Value const Purple	= 0x04;
	Value const Blue		= 0x05;
	Value const Green_Orange_Diamond = 0x06;
	Value const Yellow_Green_Point = 0x07;
	Value const DBlue_LBlue_Point = 0x08;
	Value const DBlue_Pink_BCircle = 0x09;  
	Value const Purple_Green_BCircle = 0x0a; 
	Value const Pink_Yellow_Square = 0x0b;
	Value const LBlue_Pink_Square = 0x0c; 
	Value const Purple_Yellow_Star = 0x0d; 
	Value const Green_Pink_Circle = 0x0e;
	Value const Orange_Purple_Star = 0x0f; 
	Value const Yellow_DBlue_Square = 0x10; 
	Value const Purple_LBlue_Diamond = 0x11;
	Value const DBlue_Orange_Circle = 0x12; 
	Value const LBlue_Pink_Diamond = 0x13; 
	Value const Yellow_LBlue_Star = 0x14;
	Value const Purple_Yellow_Circle = 0x15; 
	Value const Pink_Yellow_Circle = 0x16; 
	Value const Green_DBlue_Octagon = 0x17;
	Value const Purple_Orange_Circle = 0x18; 
	Value const Pink_LBlue_Square = 0x19; 
	Value const Orange_LBlue_Circle = 0x1a;
	Value const DBlue_Yellow_Circle = 0x1b;
	
	Piece::Piece(PieceId const id_, PieceId const rotation_, Value const left_, Value const right_, Value const top_, Value const bottom_) 
		: _id(id_), _rotation(rotation_)
	{
		_sides[Left] = left_; _sides[Right] = right_; 
		_sides[Top]  = top_; _sides[Bottom] = bottom_;
	}

	Piece::Piece(std::istream & file_)
	{
		file_.read(reinterpret_cast<char *>(&_id), sizeof(PieceId));
		file_.read(reinterpret_cast<char *>(&_rotation), sizeof(PieceId));
		file_.read(reinterpret_cast<char *>(&(_sides[Left])), sizeof(Value));
		file_.read(reinterpret_cast<char *>(&(_sides[Right])), sizeof(Value));
		file_.read(reinterpret_cast<char *>(&(_sides[Top])), sizeof(Value));
		file_.read(reinterpret_cast<char *>(&(_sides[Bottom])), sizeof(Value));
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

	void Piece::save(std::ofstream & file_) const
	{
		file_.write(reinterpret_cast<const char *>(&_id), sizeof(PieceId));
		file_.write(reinterpret_cast<const char *>(&_rotation), sizeof(PieceId));
		file_.write(reinterpret_cast<const char *>(&(_sides[Left])), sizeof(Value));
		file_.write(reinterpret_cast<const char *>(&(_sides[Right])), sizeof(Value));
		file_.write(reinterpret_cast<const char *>(&(_sides[Top])), sizeof(Value));
		file_.write(reinterpret_cast<const char *>(&(_sides[Bottom])), sizeof(Value));
	}

	void SavePieces(std::ofstream & file_, Pieces const & pieces_)
	{
		Pieces::size_type piecesCount = pieces_.size();
		file_.write(reinterpret_cast<const char *>(&piecesCount), sizeof(Pieces::size_type));
		for(Pieces::const_iterator i = pieces_.begin(); i != pieces_.end(); ++i)
		{
			(*i)->save(file_);
		}
	}

	void LoadPieces(std::ifstream & file_, Pieces & pieces_)
	{
		Pieces::size_type piecesCount = 0;
		file_.read(reinterpret_cast<char *>(&piecesCount), sizeof(Pieces::size_type));
		for (Pieces::size_type i = 0; i < piecesCount; ++i)
		{
			Piece * p = new Piece(file_);
			pieces_.push_back(p);
		}
	}

	Pieces::const_iterator FindPiece(Pieces const & pieces_, PieceId const & id_, PieceId const & rotation_)
	{
		for (Pieces::const_iterator i = pieces_.begin(); i != pieces_.end(); ++i)
		{
			if( (*i)->id() == id_ && (*i)->rotation() == rotation_){ return i; }
		}

		return pieces_.end();
	}
}
