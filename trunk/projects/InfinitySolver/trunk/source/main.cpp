#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <ctime>
#include <conio.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "AM_Process.hpp"
#include "AM_JobManager.hpp"
#include "AM_ThreadProcessManager.hpp"
#include "AM_NetworkProcessManager.hpp"

#include "Infinity_Piece.hpp"
#include "Infinity_Board.hpp"
#include "Infinity_Solver.hpp"

void AddPiece(Infinity::Pieces & pieces_, char id_,
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
}


void SimpleTest(int threadCount_)
{
	std::cout << "Starting simple test... " << std::endl;
	Infinity::Pieces * pieces = new Infinity::Pieces;
	Infinity::Pieces * delPieces = 0;
	{
		AutoMatic::JobManager jobManager;
		AutoMatic::ThreadProcessManager threadProcessManager(&jobManager, threadCount_);
		Infinity::Board * board = new Infinity::Board(2,2);
		
		AddPiece(*pieces, 0x01, Infinity::Edge, Infinity::Orange, Infinity::Orange, Infinity::Edge);
		AddPiece(*pieces, 0x02, Infinity::Orange, Infinity::Edge, Infinity::Orange, Infinity::Edge);
		AddPiece(*pieces, 0x03, Infinity::Edge, Infinity::Orange, Infinity::Edge, Infinity::Orange);
		AddPiece(*pieces, 0x04, Infinity::Orange, Infinity::Edge, Infinity::Edge, Infinity::Orange);

		delPieces = new Infinity::Pieces(*pieces);

		Infinity::Solver * solve = new Infinity::Solver(board, pieces);

		jobManager.queueJob(solve);

		while (!jobManager.update()){ Sleep(0); }
	}

	for (Infinity::Pieces::iterator i = delPieces->begin(); i != delPieces->end(); ++i)
	{
		delete *i;
	}

	delete delPieces;
}
void UnthreadedTest(void);
void MajorTest(int threads_, enet_uint16 port_);
int main(int argc_, char const * * argv_)
{
	int threadCount = 1;
	enet_uint16 port = 0;

	if(argc_ < 3)
	{ 
		std::cout << "Usage: " << argv_[0] << " [Thread Count] [Port]" << std::endl; 
		return EXIT_SUCCESS;
	}
	else
	{
		threadCount = boost::lexical_cast<int>(argv_[1]);
		port = boost::lexical_cast<enet_uint16>(argv_[2]);
	}

	SimpleTest(threadCount);
	Infinity::DebugOutput::ResetSolutions();
	UnthreadedTest();
	Infinity::DebugOutput::ResetSolutions();
	MajorTest(threadCount, port);
	Infinity::DebugOutput::ResetSolutions();

	return EXIT_SUCCESS;
}

class UnThreadedProcess : public AutoMatic::Process
{
public:
	UnThreadedProcess(AutoMatic::Job * job_)
		: AutoMatic::Process(0)
	{
		queueJob(job_);
	}

	virtual void queueJob(AutoMatic::Job * job_)
	{
		_jobs.push(job_);
	}

	virtual	void runJob(void)
	{
		AutoMatic::Job * job = _job;
		if(!job)
		{
			job = _jobs.front();
			_jobs.pop();
		}
		_job = job->run(this);
	}

	virtual bool update(void)
	{
		if(_job || _jobs.size() > 0)
		{
			runJob();
			return true;
		}

		return false;
	}

	virtual char const id(void) const { return 0x00; }

private:
	std::queue<AutoMatic::Job *> _jobs;
};

void SetBoard(Infinity::Pieces * pieces_)
{
	AddPiece(*pieces_, 0x00, Infinity::Edge, Infinity::Orange, Infinity::Edge, Infinity::Blue);
	AddPiece(*pieces_, 0x01, Infinity::Orange, Infinity::Blue, Infinity::Edge, Infinity::Pink);
	AddPiece(*pieces_, 0x02, Infinity::Blue, Infinity::Orange, Infinity::Edge, Infinity::Pink);
	AddPiece(*pieces_, 0x03, Infinity::Orange, Infinity::Edge, Infinity::Edge, Infinity::Blue);

	AddPiece(*pieces_, 0x04, Infinity::Edge, Infinity::Purple, Infinity::Blue, Infinity::Orange);
	AddPiece(*pieces_, 0x05, Infinity::Purple, Infinity::Pink, Infinity::Pink, Infinity::Purple);
	AddPiece(*pieces_, 0x06, Infinity::Pink, Infinity::Pink, Infinity::Pink, Infinity::Purple);
	AddPiece(*pieces_, 0x07, Infinity::Pink, Infinity::Edge, Infinity::Blue, Infinity::Blue);

	AddPiece(*pieces_, 0x08, Infinity::Edge, Infinity::Purple, Infinity::Orange, Infinity::Orange);
	AddPiece(*pieces_, 0x09, Infinity::Purple, Infinity::Pink, Infinity::Purple, Infinity::Pink);
	AddPiece(*pieces_, 0x0a, Infinity::Pink, Infinity::Purple, Infinity::Purple, Infinity::Purple);
	AddPiece(*pieces_, 0x0b, Infinity::Purple, Infinity::Edge, Infinity::Blue, Infinity::Blue);

	AddPiece(*pieces_, 0x0c, Infinity::Edge, Infinity::Orange, Infinity::Orange, Infinity::Edge);
	AddPiece(*pieces_, 0x0d, Infinity::Orange, Infinity::Orange, Infinity::Pink, Infinity::Edge);
	AddPiece(*pieces_, 0x0e, Infinity::Orange, Infinity::Blue, Infinity::Purple, Infinity::Edge);
	AddPiece(*pieces_, 0x0f, Infinity::Blue, Infinity::Edge, Infinity::Blue, Infinity::Edge);
}

void UnthreadedTest(void)
{
	std::cout << "Starting Un-Threaded Test..." << std::endl;
	Infinity::Pieces * pieces = new Infinity::Pieces;
	Infinity::Pieces * delPieces = 0;
	{
		Infinity::Board * board = new Infinity::Board(4,4);
		SetBoard(pieces);
		delPieces = new Infinity::Pieces(*pieces);
		Infinity::Solver * solve = new Infinity::Solver(board, pieces);
		UnThreadedProcess process(solve);
		
		clock_t start = clock();
		while(process.update());
		clock_t stop = clock();
		
		std::cout << "Solutions: " << Infinity::DebugOutput::Solutions() << std::endl;
		std::cout << "Time: " << stop - start << std::endl;
	}

	for (Infinity::Pieces::iterator i = delPieces->begin(); i !=delPieces->end(); ++i)
	{
		delete *i;
	}
	delete delPieces;
}

void MajorTest(int threads_, enet_uint16 port_)
{
	std::cout << "Starting major test... " << std::endl;
	Infinity::Pieces * pieces = new Infinity::Pieces;
	Infinity::Pieces * delPieces = 0;
	{
		AutoMatic::JobManager jobManager;
		AutoMatic::ThreadProcessManager threadProcessManager(&jobManager, threads_);
		//AutoMatic::NetworkProcessManager networkProcessManager(&jobManager, port_);

		AutoMatic::ThreadProcessManager threadProcessManager2(&jobManager, threads_);
		//AutoMatic::NetworkProcessManager networkProcessManager2(&jobManager, port_ + 1);

		//if(networkProcessManager.connect("localhost", port_ + 1))
		{

			Infinity::Board * board = new Infinity::Board(4,4);
			SetBoard(pieces);
			delPieces = new Infinity::Pieces(*pieces);

			Infinity::SolverFactory solverFactory(*board, *pieces);
			Infinity::Solver * solve = new Infinity::Solver(board, pieces);

			jobManager.addJobFactory(&solverFactory);
			//threadProcessManager.link(&networkProcessManager);
			//threadProcessManager2.link(&networkProcessManager2);

			clock_t start = clock();
			jobManager.queueJob(solve);
			while (!jobManager.update()){ Sleep(0); }
			clock_t stop = clock();

			std::cout << "Max pending jobs: " << jobManager._debugMaxPendingJobs << std::endl;
			std::cout << "Solutions: " << Infinity::DebugOutput::Solutions() << std::endl;
			std::cout << "Time: " << stop - start << std::endl;
		}
		//else
		//{
		//	std::cout << "Failed to connect to " << port + 1 << std::endl;
		//}
	}

	for (Infinity::Pieces::iterator i = delPieces->begin(); i !=delPieces->end(); ++i)
	{
		delete *i;
	}
	delete delPieces;
}