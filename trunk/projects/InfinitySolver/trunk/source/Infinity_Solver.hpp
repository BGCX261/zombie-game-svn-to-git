#ifndef _Infinity_solver_hpp_
#define _Infinity_solver_hpp_

#include <list>
#include <string>

#include "Infinity_Board.hpp"
#include "Infinity_Piece.hpp"

#include "AM_Job.hpp"

namespace Infinity
{
	class DebugOutput : public AutoMatic::Job
	{
	public:
		DebugOutput(void)//Infinity::Board const & board_)
			//: _board(board_)
		{}

		virtual Job * run(AutoMatic::Process * )
		{
			boost::recursive_mutex::scoped_lock printLock(_PrintMutex);
			//_board.debugOutput();
			++_Solutions;
			return 0;
		}

		virtual char getJobFactoryId(void){ return 0x00; }

		static int const Solutions(void) { return _Solutions; }

		static void ResetSolutions(void) { _Solutions = 0; }

	private:
		static boost::recursive_mutex _PrintMutex;
		static int _Solutions;
		//Infinity::Board _board;
	};

	class Solver : public AutoMatic::Job
	{
	public:
		Solver(Board const * board_, Pieces const * pieces_);
		virtual ~Solver(void);

		virtual AutoMatic::Job * run(AutoMatic::Process * process_);

		virtual char getJobFactoryId(void){ return 0x00; }

		Pieces const & pieces(void) const { return *_pieces; }
		Board const & board(void) const { return *_board; }

	private:
		Board const * _board;
		Pieces const * _pieces;
	};

	class SolverFactory : public AutoMatic::JobFactory
	{
	public:
		SolverFactory(Board const & board_, Pieces const & pieces_);

		virtual char id(void) const { return 0x00; }
		virtual AutoMatic::Job * create(std::string const & data_);
		virtual std::string create(AutoMatic::Job * job_);

	private:
		Board const _board;
		Pieces const _pieces;
	};
}

#endif // _Infinity_solver_hpp_