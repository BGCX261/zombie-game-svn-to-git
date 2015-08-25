#include <cassert>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "AM_Process.hpp"
#include "Infinity_Solver.hpp"

namespace Infinity
{
	boost::recursive_mutex DebugOutput::_PrintMutex;
	int DebugOutput::_Solutions = 0;

	Solver::Solver(Board const * board_, Pieces const * pieces_)
		: _board(board_), _pieces(pieces_)
	{
	}

	Solver::~Solver(void)
	{
		delete _board; delete _pieces;
	}

	// Input: Board, Pieces
	// Steps:
	// For each piece,
		// If piece fits
			// Place piece on the board,
				// Modify placed pieces,
				// Modify open-tiles,
			// Remove piece from pieces,
			// Start again with new board and pieces.

	AutoMatic::Job * Solver::run(AutoMatic::Process * process_)
	{
		if(0 == _pieces->size())
		{ // No more pieces to fit, board completed.
			DebugOutput * output = new DebugOutput;//*_board);
			process_->queueJob(output);
			return 0;
		}

		int similarCount = 0, minSimilarCount = 0;
		Solver * nextSolver = 0;
		
		Pieces pieces(*_pieces);
		for (Pieces::iterator currentPiece = pieces.begin(); currentPiece != pieces.end(); ++currentPiece)
		{ // 1. For each available piece,
			similarCount = 0;

			Pieces::iterator findSimilar = currentPiece, removeSimilar;
			++findSimilar;
			while(findSimilar != pieces.end())
			{ // 1.1 Remove matching pieces from available pieces.
				if( !(*findSimilar)->matches(*currentPiece) ){ ++findSimilar; }
				else 
				{
					similarCount ++;
					removeSimilar	= findSimilar; 
					++findSimilar;
					pieces.erase(removeSimilar);
				}
			}

			if(Tile const * t = _board->fit(*currentPiece))
			{ 
				// 1.2 If the piece fits,
				// 1.2.2 Place the piece on the board.
				Board * newBoard = new Board(*_board);
				newBoard->place( (*currentPiece), t->x(), t->y() );
				Pieces * newPieces = new Pieces(*_pieces);

				Pieces::iterator findSame = newPieces->begin(), removeSame;
				while (findSame != newPieces->end())
				{ // 1.2.3 Remove the piece from the available pieces.
					if( (*findSame)->id() != (*currentPiece)->id() ){ ++findSame; }
					else 
					{ 
						removeSame = findSame; 
						++findSame; 
						newPieces->erase(removeSame); 
					}
				}

				// 1.2.4 Create a new solver with the new board and new pieces,
				Solver * solver = new Solver(newBoard, newPieces);

				//if(!nextSolver || minSimilarCount < similarCount)
				//{
				//	minSimilarCount = similarCount;
				//	if(nextSolver){ process_->queueJob(nextSolver); }
				//	nextSolver = solver;
				//}
				//else 
				{
					// 1.2.5 Queue the new solver.
					process_->queueJob(solver);
				}
			}
		}

		return nextSolver;
	}

	SolverFactory::SolverFactory(Board const & board_, Pieces const & pieces_)
		: _board(board_), _pieces(pieces_)
	{}

	AutoMatic::Job * SolverFactory::create(std::string const & data_)
	{
		int dataPos = 0;
		Pieces pieces;
		Board board(_board.width(), _board.height(), _board.startX(), _board.startY());
		int piecesCount = static_cast<int>(static_cast<unsigned char>(data_[dataPos]));

		assert(piecesCount <= 255 && piecesCount >= 0);

		for (dataPos = 1; dataPos <= piecesCount; dataPos++)
		{
			for (std::list<Piece const *>::const_iterator pieceIter = _pieces.begin();
				pieceIter != _pieces.end(); ++pieceIter)
			{
				if (data_[dataPos++] == (*pieceIter)->id())
				{
					if(data_[dataPos] == (*pieceIter)->rotation())
					{
						pieces.push_back(*pieceIter);
						break;
					}
				}
			}
		}

		for (char i = 0; i < _board.width(); ++i)
		{
			for (char j = 0; j < _board.width(); ++j)
			{
				switch(data_[dataPos++])
				{
				case 'n':
					{
					}
					break;
				case 'p':
					{
						char id = data_[dataPos++];
						char rot = data_[dataPos++];

						for (std::list<Piece const *>::const_iterator pieceIter = _pieces.begin();
							pieceIter != _pieces.end(); ++pieceIter)
						{
							if (id == (*pieceIter)->id() && rot == (*pieceIter)->rotation())
							{
								board.setPiece(*pieceIter, i, j);
								break;
							}
						}
					}
					break;
				}
			}
		}

		return new Solver(new Board(board), new Pieces(pieces));
	}

	std::string SolverFactory::create(AutoMatic::Job * job_)
	{
		std::string job;
		Solver * solver = static_cast<Solver *>(job_);
		
		assert(solver->pieces().size() <= 256);

		job += static_cast<char>(solver->pieces().size());

		for (std::list<Piece const *>::const_iterator i = solver->pieces().begin();
			i != solver->pieces().end(); ++i)
		{
			job += (*i)->id();
			job += (*i)->rotation();
		}
/*
		assert(solver->board().openTiles().size() <= 256);

		job += static_cast<char>(solver->board().openTiles().size());
		for (Tiles::const_iterator i = solver->board().openTiles().begin();
			i != solver->board().openTiles().end(); ++i)
		{
			job += (*i)._x;
			job += (*i)._y;
			job += (*i)._nonEmpty;
			job += (*i)._edges;
			
			for (int j = 0; j < All_Sides; ++j)
			{
				job += static_cast<char>((*i)._sides[j]);
			}
		}
*/
		for (char i = 0; i < _board.width(); ++i)
		{
			for (char j = 0; j < _board.width(); ++j)
			{
				Piece const * piece = _board.getPiece(i,j);
				if(piece)
				{
					job += "p";
					job += piece->id();
					job += piece->rotation();
				}
				else
				{
					job += "n";
				}
			}
		}
		
		return job;
	}
}
