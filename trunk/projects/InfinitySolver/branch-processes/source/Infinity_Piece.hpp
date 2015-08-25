#ifndef _Infinity_Piece_hpp_
#define _Infinity_Piece_hpp_

#include <list>
#include <fstream>

namespace Infinity
{
	enum Side { Left = 0, Right, Top, Bottom, All_Sides };
	typedef unsigned char Value;	
	extern Value const Orange, Pink, Purple, Blue, 
		Green_Orange_Diamond, Yellow_Green_Point, DBlue_LBlue_Point,
		DBlue_Pink_BCircle, Purple_Green_BCircle, Pink_Yellow_Square,
		LBlue_Pink_Square, Purple_Yellow_Star, Green_Pink_Circle,
		Orange_Purple_Star, Yellow_DBlue_Square, Purple_LBlue_Diamond,
		DBlue_Orange_Circle, LBlue_Pink_Diamond, Yellow_LBlue_Star, 
		Purple_Yellow_Circle, Pink_Yellow_Circle, Green_DBlue_Octagon,
		Purple_Orange_Circle, Pink_LBlue_Square, Orange_LBlue_Circle,
		DBlue_Yellow_Circle,
		Edge, Empty, All_Special_Colours;

	typedef unsigned int PieceId;

	typedef unsigned int CooOrd;

	class Piece
	{
	public:
		Piece(PieceId const id_, PieceId const rotation_, Value const left_, Value const right_, Value const top_, Value const bottom_);
		Piece(std::istream & file_);

		bool matches(Piece const * piece_) const;

		PieceId const id(void) const { return _id; }
		PieceId const rotation(void) const { return _rotation; }

		Value const side(Side const side_) const;
		Value const * sides(void) const { return _sides; }
		Piece * rotateCW(void) const;

		void save(std::ofstream & file_) const;

	private:
		PieceId _id, _rotation;
		Value _sides[All_Sides];

		Piece(Piece const & piece_);
		bool operator==(Piece const & piece_) const;
		Piece const & operator=(Piece const & piece_);
	};

	typedef std::list<Piece const *> Pieces;

	Pieces::const_iterator FindPiece(Pieces const & pieces_, PieceId const & id_, PieceId const & rotation_);

	void SavePieces(std::ofstream & file_, Pieces const & pieces_);

	void LoadPieces(std::ifstream & file_, Pieces & pieces_);
}

#endif // _Infinity_Piece_hpp_