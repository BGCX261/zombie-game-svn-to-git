#include <cstdlib>
#include <cassert>

#include <set>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Infinity_Board.hpp"

#include "AM_Memory.h"

namespace Infinity
{
	Infinity::Piece * AddPiece(Infinity::Pieces & pieces_, PieceId id_,
		Infinity::Value l_, Infinity::Value r_, Infinity::Value t_, Infinity::Value b_)
	{

		Infinity::Piece * piece[4];
		piece[0] = new Infinity::Piece(id_, 0x01, l_, r_, t_, b_);

		piece[1] = piece[0]->rotateCW();

		piece[2] = piece[1]->rotateCW();

		piece[3] = piece[2]->rotateCW();

		pieces_.push_back(piece[0]);
		pieces_.push_back(piece[1]);
		if(!piece[0]->matches(piece[2]))
		{
			pieces_.push_back(piece[2]);
			pieces_.push_back(piece[3]);
		}
		else
		{
			delete piece[2];
			delete piece[3];
		}

		return piece[0];
	}

	EdgeFlags CountNonEmptyAndEdges(Value const * sides_)
	{ 
		EdgeFlags edgeFlags = 0x11; 
		for(int i = 0; i < All_Sides; ++i)
		{ 
			if(sides_[i] != Empty)
			{ 
				edgeFlags |= ((edgeFlags & 0xf0) << 1);
				if(sides_[i] == Edge){ edgeFlags |= ((edgeFlags & 0x0f) << 1); }
			} 
		} 

		return edgeFlags;
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

//****************************************************************************//
	SortPieces::SortPieces(Pieces * pieces_, int seed_)
		: _betterIfMoreEdges(true), _betterIfCommon(true)
	{
		srand(seed_);
		analyseValues(pieces_);
		pieces_->sort(*this);
	}

	bool SortPieces::operator()(Piece const * lhs_, Piece const * rhs_) const
	{
		// Compare the number of edges.
		EdgeFlags l = CountNonEmptyAndEdges(lhs_->sides());
		EdgeFlags r = CountNonEmptyAndEdges(rhs_->sides());
		if(l < r){ return _betterIfMoreEdges; } // Left < edges than Right.
		if(l > r){ return !_betterIfMoreEdges; }
		
		// Same number of edges...

		// Compare the how many pieces are the same colour...

		//std::set<Value> lhsSameness;
		//lhsSameness.insert(lhs_->side(Left));
		//lhsSameness.insert(lhs_->side(Right));
		//lhsSameness.insert(lhs_->side(Top));
		//lhsSameness.insert(lhs_->side(Bottom));

		//std::set<Value> rhsSameness;
		//rhsSameness.insert(rhs_->side(Left));
		//rhsSameness.insert(rhs_->side(Right));
		//rhsSameness.insert(rhs_->side(Top));
		//rhsSameness.insert(rhs_->side(Bottom));

		// Compare how 'common' each piece is...
		// I.e., how often...
		unsigned long lhsValues = _countValues[lhs_->side(Left)] + // The left side colour occurs,
			_countValues[lhs_->side(Right)] +  // the right side colour occurs,
			_countValues[lhs_->side(Top)] +  // the top side colour occurs and
			_countValues[lhs_->side(Bottom)]; // the bottom side colour occurs.

		unsigned long rhsValues = _countValues[rhs_->side(Left)] + 
			_countValues[rhs_->side(Right)] + 
			_countValues[rhs_->side(Top)] + 
			_countValues[rhs_->side(Bottom)];

		// A piece is more common if it has a high lhsValues and a high _sameness...
		// Except.. for some reason it takes for ever if I multiply (even if _betterIfCommon is false...)
		//double lhsValuesDiv = lhsValues / static_cast<double>(_sameness[lhsSameness.size()]);
		//double rhsValuesDiv = rhsValues / static_cast<double>(_sameness[rhsSameness.size()]);

		//lhsValues *= _sameness[lhsSameness.size()];
		//rhsValues *= _sameness[rhsSameness.size()];

		rhsValues = rand() % (lhsValues + rhsValues);

		if(lhsValues > rhsValues){ return _betterIfCommon; }
		if(lhsValues < rhsValues){ return !_betterIfCommon; }
		
		return lhs_->id() < rhs_->id();
	}

	void SortPieces::analyseValues(Pieces * pieces_)
	{
		for (Pieces::iterator i = pieces_->begin(); i != pieces_->end(); ++i)
		{
			_countValues[(*i)->side(Left)]++;
			_countValues[(*i)->side(Right)]++;
			_countValues[(*i)->side(Top)]++;
			_countValues[(*i)->side(Bottom)]++;

			//std::set<Value> sameness;
			//sameness.insert((*i)->side(Left));
			//sameness.insert((*i)->side(Right));
			//sameness.insert((*i)->side(Top));
			//sameness.insert((*i)->side(Bottom));
			
			//_sameness[sameness.size()]++;
		}
	}

	void AnalysePiece(Piece const * piece, std::set<Value> & colourCount, std::map<int, int> & similarPiecesCount,
		std::map<Value, int> & colourUsage, std::map<Value, std::map<int, int> > & colourOnPieceUsage,
		std::map<Value, std::map<int, int> > & colourOfPieceUsage)
	{
		colourCount.insert(piece->side(Left));
		colourCount.insert(piece->side(Right));
		colourCount.insert(piece->side(Top));
		colourCount.insert(piece->side(Bottom));

		std::set<Value> sameCount;
		sameCount.insert(piece->side(Left));
		sameCount.insert(piece->side(Right));
		sameCount.insert(piece->side(Top));
		sameCount.insert(piece->side(Bottom));

		similarPiecesCount[sameCount.size()]++;

		std::map<Value, int> cantThinkOfName;
		cantThinkOfName[piece->side(Left)]++;
		cantThinkOfName[piece->side(Right)]++;
		cantThinkOfName[piece->side(Top)]++;
		cantThinkOfName[piece->side(Bottom)]++;

		(colourUsage[piece->side(Left)])++;
		(colourUsage[piece->side(Right)])++;
		(colourUsage[piece->side(Top)])++;
		(colourUsage[piece->side(Bottom)])++;

		for (std::set<Value>::iterator k = sameCount.begin(); k != sameCount.end(); ++k)
		{					
			((colourOnPieceUsage[*k])[sameCount.size()])++;
		}				

		for (std::map<Value, int>::iterator k = cantThinkOfName.begin(); k != cantThinkOfName.end(); ++k)
		{
			(colourOfPieceUsage[(*k).first])[(*k).second]++;
		}
	}

	void PrintAnalysis(std::ofstream & output, Value colours_, std::set<Value> & colourCount, 
		std::map<int, int> & similarPiecesCount,	std::map<Value, int> & colourUsage, 
		std::map<Value, std::map<int, int> > & colourOnPieceUsage,
		std::map<Value, std::map<int, int> > & colourOfPieceUsage)
	{
		assert(colourCount.size() <= colours_ + 1);
		std::cout << "Colours used: " << colourCount.size() << std::endl;
		output << "Colours used: " << colourCount.size() << std::endl;
		std::cout << "Colour Count: " << std::endl;
		output << "Colour Count: " << std::endl;
		for (std::map<Value, int>::iterator i = colourUsage.begin(); i != colourUsage.end(); ++i)
		{
			std::cout << (int)(*i).first << ": " << (*i).second << std::endl;
			output << (int)(*i).first << ": " << (*i).second << std::endl;
		}

		std::cout << "Sameness Count: " << std::endl;
		output << "Sameness Count: " << std::endl;
		for (std::map<int, int>::iterator i = similarPiecesCount.begin(); i != similarPiecesCount.end(); ++i)
		{
			std::cout << (int)(*i).first << " colour(s) : " << (*i).second << std::endl;
			output << (int)(*i).first << " colour(s) : " << (*i).second << std::endl;
		}

		std::cout << "Colour On Piece Usage: " << std::endl;
		output << "Colour On Piece Usage: " << std::endl;
		for (std::map<Value, std::map<int, int > >::iterator i = colourOnPieceUsage.begin(); i != colourOnPieceUsage.end(); ++i)
		{
			std::cout << (int)(*i).first << ": " << std::endl;
			output << (int)(*i).first << ": " << std::endl;
			for(std::map<int, int >::iterator j = (*i).second.begin(); j != (*i).second.end(); ++j)
			{
				std::cout << "   " << (*j).first << ": " << (*j).second << std::endl;
				output << "   " << (*j).first << ": " << (*j).second << std::endl;
			}
		}

		std::cout << "Colour Of Piece Usage: " << std::endl;
		output << "Colour Of Piece Usage: " << std::endl;
		for (std::map<Value, std::map<int, int > >::iterator i = colourOfPieceUsage.begin(); i != colourOfPieceUsage.end(); ++i)
		{
			std::cout << (int)(*i).first << ": " << std::endl;
			output << (int)(*i).first << ": " << std::endl;
			for(std::map<int, int >::iterator j = (*i).second.begin(); j != (*i).second.end(); ++j)
			{
				std::cout << "   " << (*j).first << ": " << (*j).second << std::endl;
				output << "   " << (*j).first << ": " << (*j).second << std::endl;
			}
		}

		std::cout << std::endl;
		output << "*******************" << std::endl;
	}

	void PiecesAnalyse(Pieces * pieces_, Value colours_)
	{
		std::ofstream output;

		output.open("debugAnalysis.txt", std::ios::out | std::ios::app);

		std::set<Value> colourCount;
		std::map<Value, int> colourUsage;
		std::map<int, int> similarPiecesCount;

		// For each colour, how often does it occur for pieces with 1,2,3, or 4 colours.
		std::map<Value, std::map<int, int> > colourOnPieceUsage;
		std::map<Value, std::map<int, int> > colourOfPieceUsage;

		for(Pieces::iterator i = pieces_->begin(); i != pieces_->end(); ++i)
		{
			Piece const * piece = *i;
			AnalysePiece(piece, colourCount, similarPiecesCount, colourUsage, colourOnPieceUsage, colourOfPieceUsage);
		}

		PrintAnalysis(output, colours_, colourCount, similarPiecesCount, colourUsage, colourOnPieceUsage, colourOfPieceUsage);
	}

	void Board::analyse(Value colours_) const
	{
		std::ofstream output;

		output.open("debugAnalysis.txt", std::ios::out | std::ios::app);

		std::set<Value> colourCount;
		std::map<Value, int> colourUsage;
		std::map<int, int> similarPiecesCount;
		
		// For each colour, how often does it occur for pieces with 1,2,3, or 4 colours.
		std::map<Value, std::map<int, int> > colourOnPieceUsage;
		std::map<Value, std::map<int, int> > colourOfPieceUsage;

		for (CooOrd i = 0; i < _width; ++i)
		{
			for (CooOrd j = 0; j < _height; ++j)
			{
				Piece const * piece = getPiece(i, j);
				AnalysePiece(piece, colourCount, similarPiecesCount, colourUsage, colourOnPieceUsage, colourOfPieceUsage);
			}
		}

		PrintAnalysis(output, colours_, colourCount, similarPiecesCount, colourUsage, colourOnPieceUsage, colourOfPieceUsage);
	}

	Infinity::Pieces * Board::GeneratePieces(unsigned int seed_, CooOrd const width_, CooOrd const height_, Value const colours_)
	{
		bool debugPreRand = false;

		Board tempBoard(width_, height_);
		Pieces * pieces = new Pieces;
		srand(seed_);
		PieceId id = 0x00;
		for (CooOrd i = 0; i < width_; ++i)
		{
			for (CooOrd j = 0; j < height_; ++j)
			{
				Tile tile(i, j);
				tempBoard.setTileBounds(tile);
				
				Value l, r, b, t;

				// Always generating the random value seems to mess things up?
					// Or is it actually working, but simply by chance producing a difficult puzzle?
					// I have a sad suspicion that it is the latter.
				if(debugPreRand)
				{
					Value colourL = (rand() % colours_) + All_Special_Colours;
					Value colourR = (rand() % colours_) + All_Special_Colours;
					Value colourT = (rand() % colours_) + All_Special_Colours;
					Value colourB = (rand() % colours_) + All_Special_Colours;

					l = (tile.side(Left) != Empty ? tile.side(Left) : colourL);
					r = (tile.side(Right) != Empty ? tile.side(Right) : colourR);
					t = (tile.side(Top) != Empty ? tile.side(Top) : colourT);
					b = (tile.side(Bottom) != Empty ? tile.side(Bottom) : colourB);
				}
				else
				{
					l = (tile.side(Left) != Empty ? tile.side(Left) : (rand() % colours_) + All_Special_Colours);
					r = (tile.side(Right) != Empty ? tile.side(Right) : (rand() % colours_) + All_Special_Colours);
					t = (tile.side(Top) != Empty ? tile.side(Top) : (rand() % colours_) + All_Special_Colours);
					b = (tile.side(Bottom) != Empty ? tile.side(Bottom) : (rand() % colours_) + All_Special_Colours);
				}
				
				Piece * piece = AddPiece(*pieces, id++, l, r, t, b);
				tempBoard.setPiece(piece, i, j);
			}
		}

		if(!tempBoard.verify())
		{
			return 0;
		}

		tempBoard.analyse(colours_);

		tempBoard.debugOutput();

		SortPieces sort(pieces, seed_);

		return pieces;
	}

	Board::Board(CooOrd const width_, CooOrd const height_, CooOrd const startX_, CooOrd const startY_)
		: _width(width_), _height(height_), _startX(startX_), _startY(startY_), _board(0)
	{
		createNewBoard();
		for(CooOrd i = 0; i < _width*_height; ++i){ _board[i] = 0; }
		addToOpenTiles(_startX, _startY);
	}

	Board::Board(Board const & board_)
		: _width(board_._width), _height(board_._height	), _startX(board_._startX), _startY(board_._startY), _board(0)
		, _openTiles(board_._openTiles)
	{
		createNewBoard();
		for(CooOrd i = 0; i < _width*_height; ++i){ _board[i] = board_._board[i]; }
	}

	Board::Board(std::ifstream & file_, Pieces const & pieces_)
		: _board(0)
	{
		file_.read(reinterpret_cast<char *>(&_width), sizeof(CooOrd));
		file_.read(reinterpret_cast<char *>(&_height), sizeof(CooOrd));
		file_.read(reinterpret_cast<char *>(&_startX), sizeof(CooOrd));
		file_.read(reinterpret_cast<char *>(&_startY), sizeof(CooOrd));
		createNewBoard();
		for(CooOrd i = 0; i < _width*_height; ++i){ _board[i] = 0; }

		for (CooOrd i = 0; i < _width; ++i)
		{
			for (CooOrd j = 0; j < _height; ++j)
			{
				PieceId piece = 0;
				file_.read(reinterpret_cast<char *>(&piece), sizeof(PieceId));
				if(1 == piece)
				{
					file_.read(reinterpret_cast<char *>(&piece), sizeof(PieceId));
					PieceId rot = 0;
					file_.read(reinterpret_cast<char *>(&rot), sizeof(PieceId));
					Pieces::const_iterator p = FindPiece(pieces_, piece, rot);
					if(p != pieces_.end())
					{
						place(*p, i, j);
					}
				}
			}
		}
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

	void Board::save(std::ofstream & file_) const
	{
		file_.write(reinterpret_cast<const char *>(&_width), sizeof(CooOrd));
		file_.write(reinterpret_cast<const char *>(&_height), sizeof(CooOrd));
		file_.write(reinterpret_cast<const char *>(&_startX), sizeof(CooOrd));
		file_.write(reinterpret_cast<const char *>(&_startY), sizeof(CooOrd));
		for (CooOrd i = 0; i < _width; ++i)
		{
			for (CooOrd j = 0; j < _height; ++j)
			{
				Piece const * p = getPiece(i, j);
				if(p)
				{
					PieceId piece = 1;
					file_.write(reinterpret_cast<const char *>(&piece), sizeof(PieceId));
					piece = p->id();
					file_.write(reinterpret_cast<const char *>(&piece), sizeof(PieceId));
					piece = p->rotation();
					file_.write(reinterpret_cast<const char *>(&piece), sizeof(PieceId));
				}
				else
				{
					PieceId noPiece = 0;
					file_.write(reinterpret_cast<const char *>(&noPiece), sizeof(PieceId));
				}
			}
		}
	}

	Piece const * Board::getPiece(CooOrd const x_, CooOrd const y_) const
	{
		return _board[y_*_width + x_];
	}

	void Board::setPiece(Piece const * piece_, CooOrd const x_, CooOrd const y_)
	{
		assert(!isEdge(x_,y_));
		_board[y_*_width + x_] = piece_;
	}

	bool Board::isEdge(CooOrd const x_, CooOrd const y_) const
	{
		return (x_ < 0 || x_ >= _width || y_ < 0 || y_ >= _height);
	}

	void Board::place(Piece const * piece_, CooOrd const x_, CooOrd const y_)
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

	void Board::setTileBounds(Tile & tile_)
	{
		CooOrd const x = tile_.x();
		CooOrd const y = tile_.y();
		setTileSide(tile_, x-1, y, Left);
		setTileSide(tile_, x+1, y, Right);
		setTileSide(tile_, x, y+1, Top);
		setTileSide(tile_, x, y-1, Bottom);
	}

	void Board::setTileSide(Tile & tile_, CooOrd const x_, CooOrd const y_, Side const side_)
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

	void Board::addToOpenTiles(CooOrd const x_, CooOrd const y_)
	{
		if(!isEdge(x_,y_))
		{
			Piece const * piece = getPiece(x_,y_);
			if(0 == piece)
			{
				Tile openTile(x_,y_);
				
				setTileBounds(openTile);

				removeFromOpenTiles(openTile);

				openTile.edgeFlags(CountNonEmptyAndEdges(openTile.sides()));

				_openTiles.insert(openTile);
			}
		}
	}

	void Board::removeFromOpenTiles(Tile const & tile_)
	{
		Tiles::iterator i = std::find(_openTiles.begin(), _openTiles.end(), tile_);
		if(i != _openTiles.end()){ _openTiles.erase(i); }
	}

	void Board::debugOutput(void) const
	{
		std::ofstream output;

		output.open("debugAnalysis.txt", std::ios::out | std::ios::app);
		
		

		for (int i = (_height - 1); i >= 0; --i)
		{
			for (CooOrd j = 0; j < _width; ++j)
			{
				PieceId id = 0;
				if(0 != _board[i * _width + j])
				{
					id = _board[i * _width + j]->id();
				}

				output.width(3);
				std::streamsize oldWidth = std::cout.width(3);
				std::cout << id << "  ";
				output << id << "  ";
				std::cout.width(oldWidth);
			}
			std::cout << std::endl;
			output << std::endl;
		}

		std::cout << std::endl;
		output << std::endl;

		for (int i = (_height - 1); i >= 0; --i)
		{
			for (int row = 0; row < 3; ++row)
			{
				for (CooOrd j = 0; j < _width; ++j)
				{
					//assert(_board[i * _width + j]);
					Piece const * piece = _board[i * _width + j];
					Value l=Empty,r=Empty,t=Empty,b=Empty;
					if(0 != piece)
					{
						l = piece->side(Left);
						r = piece->side(Right);
						t = piece->side(Top);
						b = piece->side(Bottom);
					}

					if(row == 2)
					{
						std::cout << ' ' << b << "  ";
						output << ' ' << b << "  ";
					}
					else if(row == 1)
					{ 
						std::cout << l << ' ' << r << ' ';
						output << l << ' ' << r << ' ';
					}
					else if(row == 0)
					{
						std::cout << ' ' << t << "  ";
						output << ' ' << t << "  ";						
					}						
				}
				std::cout << std::endl;
				output << std::endl;
			}		

			std::cout << std::endl;
			output << std::endl;
		}
		
	}

	bool Board::verify(void) const
	{
		std::set<Piece const *> pieces;
		std::set<PieceId> ids;

		for (CooOrd i = 0; i < _height; ++i)
		{
			for (CooOrd j = 0; j < _width; ++j)
			{
				Piece const * piece = getPiece(i,j);
				bool inserted = pieces.insert(piece).second;
				assert(inserted);
				if(!inserted)
				{
					std::cout << "Verification failed, non-unique piece!" << std::endl;
					return false;
				}

				inserted = ids.insert(piece->id()).second;
				assert(inserted);
				if(!inserted)
				{
					std::cout << "Verification failed, non-unique id!" << std::endl;
					return false;
				}

				if(isEdge(i - 1, j))
				{ 
					assert(piece->side(Left) == Edge);
					if(piece->side(Left) != Edge)
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}
				else 
				{ 
					Piece const * leftPiece = getPiece(i - 1, j); 
					assert(leftPiece);
					assert(leftPiece->side(Right) == piece->side(Left));
					if(!leftPiece || leftPiece->side(Right) != piece->side(Left))
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}

				if(isEdge(i + 1, j))
				{ 
					assert(piece->side(Right) == Edge); 
					if(piece->side(Right) != Edge)
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}
				else 
				{ 
					Piece const * rightPiece = getPiece(i + 1, j); 
					assert(rightPiece);
					assert(rightPiece->side(Left) == piece->side(Right));
					if(!rightPiece || rightPiece->side(Left) != piece->side(Right))
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}

				if(isEdge(i, j - 1))
				{ 
					assert(piece->side(Bottom) == Edge); 
					if(piece->side(Bottom) != Edge)
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}
				else 
				{ 
					Piece const * bottomPiece = getPiece(i, j - 1); 
					assert(bottomPiece);
					assert(bottomPiece->side(Top) == piece->side(Bottom));
					if(!bottomPiece || bottomPiece->side(Top) != piece->side(Bottom))
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}

				if(isEdge(i, j + 1))
				{ 
					assert(piece->side(Top) == Edge); 
					if(piece->side(Top) != Edge)
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}
				else 
				{ 
					Piece const * topPiece = getPiece(i, j + 1); 
					assert(topPiece);
					assert(topPiece->side(Bottom) == piece->side(Top));
					if(!topPiece || topPiece->side(Bottom) != piece->side(Top))
					{
						std::cout << "Verification failed." << std::endl;
						return false;
					}
				}
			}
		}

		assert(pieces.size() == _width * _height);
		if(pieces.size() != _width * _height)
		{
			std::cout << "Verficiation failed, not enough pieces!" << std::endl;
			return false;
		}

		std::cout << "Board Verified!" << std::endl;
		return true;
	}
}
