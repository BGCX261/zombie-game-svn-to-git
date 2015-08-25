#include <cassert>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "AM_Process.hpp"
#include "Infinity_Solver.hpp"

#include "AM_Memory.h"

namespace Infinity
{
	int _DebugSavedCount = 0;

	boost::recursive_mutex Solver::_PrintMutex;
	bool Solver::_SolutionFound = false;
	bool Solver::_TerminateOnFirstSolution = true;
	clock_t Solver::_DebugStartTime;
	int DebugOutput::_Solutions = 0;

	std::ofstream Solver::_DebugSaveFile;
	bool Solver::_DebugSave = false;

	Solver::Solver(Board const * board_, Pieces const * pieces_, int priority_)
		: AutoMatic::Job(priority_), _board(board_), _pieces(pieces_), _currentPieceIndex(0)
	{
		_runPieces = new Pieces(*_pieces);

		// TODO: Sort the pieces based on the open tile?
		// Or is that already done (I think it is...).

		_currentPiece = _runPieces->begin();	
	}

	Solver::Solver(std::ifstream & file_, Board const * board_, Pieces const * pieces_)
		: AutoMatic::Job(1), _board(board_), _pieces(pieces_), _currentPieceIndex(0)
	{
		_runPieces = new Pieces;
		file_.read(reinterpret_cast<char *>(&_currentPieceIndex), sizeof(int));
		Pieces::size_type size = 0;
		file_.read(reinterpret_cast<char *>(&size), sizeof(Pieces::size_type));
		for (int i = 0; i < size; ++i)
		{
			PieceId id = 0, rot = 0;
			file_.read(reinterpret_cast<char *>(&id), sizeof(PieceId));
			file_.read(reinterpret_cast<char *>(&rot), sizeof(PieceId));
			Pieces::const_iterator p = FindPiece(*_pieces, id, rot);
			if(p != _pieces->end())
			{
				_runPieces->push_back(*p);
			}
		}
	}

	Solver::~Solver(void)
	{
		delete _board; delete _pieces; delete _runPieces;
	}

	void Solver::save(std::ofstream & file_) const
	{
		int solver = 1;
		file_.write(reinterpret_cast<const char *>(&solver), sizeof(int)); // Note: This isn't read by the constructor...
		file_.write(reinterpret_cast<const char *>(&_currentPieceIndex), sizeof(int));
		Pieces::size_type size = _runPieces->size();
		file_.write(reinterpret_cast<const char *>(&size), sizeof(Pieces::size_type));
		for (Pieces::const_iterator i = _runPieces->begin(); i != _runPieces->end(); ++i)
		{
			PieceId id = (*i)->id();
			PieceId rot = (*i)->rotation();
			file_.write(reinterpret_cast<const char *>(&id), sizeof(PieceId));
			file_.write(reinterpret_cast<const char *>(&rot), sizeof(PieceId));
		}
	}

	std::list<Solver *> LoadSolvers(std::ifstream & file_, Board const * board_, Pieces const * pieces_)
	{
		std::list<Solver *> solvers;

		int solver = 0;
		file_.read(reinterpret_cast<char *>(&solver), sizeof(int));

		while (solver != 0)
		{
			Solver * s = new Solver(file_, new Board(*board_), new Pieces(*pieces_));
			solvers.push_back(s);

			file_.read(reinterpret_cast<char *>(&solver), sizeof(int));
		}

		return solvers;
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

	boost::recursive_mutex debugSaveMutex;

	AutoMatic::Job * Solver::run(AutoMatic::Process * process_)
	{
		if(_DebugSave)
		{
			boost::recursive_mutex::scoped_lock debugSaveLock(debugSaveMutex);
			save(_DebugSaveFile);
			_DebugSavedCount++;
			return 0;
		}

		if(_TerminateOnFirstSolution && _SolutionFound)
		{ 
			return 0; 
		}

		if(0 == _pieces->size())
		{ // No more pieces to fit, board completed.
			if(!_board->verify())
			{
				std::cout << "Fail" << std::endl;
				return 0;
			}

			DebugOutput * output = new DebugOutput;
			process_->queueJob(output);

			boost::recursive_mutex::scoped_lock printLock(_PrintMutex);
			std::cout << "Solution, " << clock() - _DebugStartTime << std::endl;
			_SolutionFound = true;
			_board->debugOutput();
			return 0;
		}

		unsigned long newPriority = 0, sameCount = 0;
		
		while (_currentPiece != _runPieces->end())
		{ // 1. For each available piece,
			sameCount = 0;
			Pieces::iterator findSimilar = _currentPiece, removeSimilar;
			++findSimilar;
			while(findSimilar != _runPieces->end())
			{ // 1.1 Remove matching pieces from available pieces.
				if( !(*findSimilar)->matches(*_currentPiece) ){ ++findSimilar; }
				else 
				{
					removeSimilar	= findSimilar; 
					++findSimilar;
					_runPieces->erase(removeSimilar);
					sameCount++;
				}
			}

			if(Tile const * t = _board->fit(*_currentPiece))
			{ 
				// 1.2 If the piece fits,
				// 1.2.2 Place the piece on the board.
				Board * newBoard = new Board(*_board);
				newBoard->place( (*_currentPiece), t->x(), t->y() );
				Pieces * newPieces = new Pieces(*_pieces);

				Pieces::iterator findSame = newPieces->begin(), removeSame;
				while (findSame != newPieces->end())
				{ // 1.2.3 Remove the piece from the available pieces.
					if( (*findSame)->id() != (*_currentPiece)->id() ){ ++findSame; }
					else 
					{ 
						removeSame = findSame; 
						++findSame; 
						newPieces->erase(removeSame); 
					}
				}

				++_currentPiece; ++_currentPieceIndex;

				// 1.2.4 Create a new solver with the new board and new pieces,
				Solver * solver = new Solver(newBoard, newPieces, priority() + (newPriority++) * sameCount);
				solver->parent(this);
				return solver;				
			}
			else
			{
				++_currentPiece; ++_currentPieceIndex;
			}			
		}

		//std::cout << "Fail, can't fit piece." << std::endl;

		return 0;
	}
}
