#ifndef _Infinity_solver_hpp_
#define _Infinity_solver_hpp_

#include <list>
#include <string>
#include <ctime>

#include "Infinity_Board.hpp"
#include "Infinity_Piece.hpp"

#include "AM_Job.hpp"

namespace Infinity
{
	class DebugOutput : public AutoMatic::Job
	{
	public:
		DebugOutput(void)
			: AutoMatic::Job(99999)
		{}

		virtual Job * run(AutoMatic::Process * )
		{
			++_Solutions;
			return 0;
		}

		virtual unsigned char getJobFactoryId(void){ return 0x00; }

		static int const Solutions(void) { return _Solutions; }

		static void ResetSolutions(void) { _Solutions = 0; }

	private:
		static int _Solutions;
	};

	class Solver : public AutoMatic::Job
	{
	public:
		Solver(Board const * board_, Pieces const * pieces_, int priority_);
		Solver(std::ifstream & file_, Board const * board_, Pieces const * pieces_);

		virtual ~Solver(void);

		virtual Job * run(AutoMatic::Process * process_);

		virtual unsigned char getJobFactoryId(void){ return 0x00; }

		Pieces const & pieces(void) const { return *_pieces; }
		Board const & board(void) const { return *_board; }

		void save(std::ofstream & file_) const;

	private:
		Board const * _board;
		Pieces const * _pieces;
		Pieces * _runPieces;
		int _currentPieceIndex;
		Pieces::iterator _currentPiece;
		static boost::recursive_mutex _PrintMutex;
		
	public:
		static bool _SolutionFound, _TerminateOnFirstSolution;
		static clock_t _DebugStartTime;

		static std::ofstream _DebugSaveFile;
		static bool _DebugSave;


		Pieces * runPieces(void){ return _runPieces; }
		int currentPieceIndex(void){ return _currentPieceIndex; }
	};

	std::list<Solver *> LoadSolvers(std::ifstream & file_, Board const * board_, Pieces const * pieces_);

	extern int _DebugSavedCount;
}

#endif // _Infinity_solver_hpp_