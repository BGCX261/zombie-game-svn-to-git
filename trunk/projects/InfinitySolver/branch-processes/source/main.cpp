#pragma warning( disable:4127 )

#include <ctime>
#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "AM_Process.hpp"
#include "AM_JobManager.hpp"
#include "AM_ThreadProcess.hpp"
#include "AM_ThreadProcessManager.hpp"

#include "Infinity_Piece.hpp"
#include "Infinity_Board.hpp"
#include "Infinity_Solver.hpp"

#include "Eternity.hpp"

#include "AM_Memory.h"

void SaveTest(void);

void SimpleTest(int threadCount_)
{
	std::cout << "Starting simple test... " << std::endl;
	Infinity::Board * board = 0;
	Infinity::Pieces * pieces = new Infinity::Pieces;
	Infinity::Pieces * delPieces = 0;
	Infinity::Solver * solve = 0;
	{
		AutoMatic::JobManager jobManager;
		
		AutoMatic::ThreadProcessManager threadProcessManager(&jobManager, threadCount_);
		board = new Infinity::Board(2,2);
		
		Infinity::AddPiece(*pieces, 0x01, Infinity::Edge, Infinity::Orange, Infinity::Orange, Infinity::Edge);
		Infinity::AddPiece(*pieces, 0x02, Infinity::Orange, Infinity::Edge, Infinity::Orange, Infinity::Edge);
		Infinity::AddPiece(*pieces, 0x03, Infinity::Edge, Infinity::Orange, Infinity::Edge, Infinity::Orange);
		Infinity::AddPiece(*pieces, 0x04, Infinity::Orange, Infinity::Edge, Infinity::Edge, Infinity::Orange);

		delPieces = new Infinity::Pieces(*pieces);

		solve = new Infinity::Solver(board, pieces, 1);
		jobManager._debugMaxProcesses = threadCount_;
		jobManager.queueJob(solve);
		jobManager.attach();
		jobManager.waitUntilTerminated();
	}

	for (Infinity::Pieces::iterator i = delPieces->begin(); i != delPieces->end(); ++i)
	{
		delete *i;
	}

	delete delPieces;
}

void SetPieces(Infinity::Pieces * pieces_);
void MajorTest(Infinity::Pieces * pieces_, Infinity::Board * board_, int threads_);
void LoadFile(std::ifstream & loadFile, int threads_);

int main(int argc_, char const * * argv_)
{
	int threadCount = 1;
	int width = 1, height = 1, seed = 1, values = 1;
	
	SaveTest();

	if(argc_ == 3)
	{
		threadCount = boost::lexical_cast<int>(argv_[1]);

		std::ifstream loadFile;
		loadFile.open(argv_[2], std::ios::binary | std::ios::in);

		if(!loadFile.is_open())
		{ 
			std::cout << "Could not open " << argv_[2] << std::endl;
			return EXIT_FAILURE; 
		}
		
		LoadFile(loadFile, threadCount);
	}
	else if(argc_ < 6)
	{ 
		std::cout << "Usage: " << argv_[0] << " [Thread Count] [Width] [Height] [Seed] [Number of Values]" << std::endl; 
		return EXIT_SUCCESS;
	}
	else
	{
		threadCount = boost::lexical_cast<int>(argv_[1]);
		width = boost::lexical_cast<int>(argv_[2]);
		height = boost::lexical_cast<int>(argv_[3]);
		seed = boost::lexical_cast<int>(argv_[4]);
		values = boost::lexical_cast<int>(argv_[5]);

		std::ofstream output;
		output.open("debugAnalysis.txt", std::ios::out | std::ios::app);
		output << "Threads: " << threadCount << " Width: " << width << " Height: " << height << " Seed: " << seed << " Values: " << values << std::endl;
	}

	if(false)
	{
		std::cout << "Simple 1" << std::endl;
		SimpleTest(threadCount);
	}
	
	if(false)
	{
		Infinity::Pieces * pieces = new Infinity::Pieces;
		SetPieces(pieces);
		Infinity::SortPieces sort(pieces, seed);
		Infinity::Board * board = new Infinity::Board(4,4);
		std::cout << "Major 1" << std::endl;
		MajorTest(pieces, board, threadCount);
	}

	if(false)
	{
		std::cout << "Major 2" << std::endl;
		Infinity::Pieces * pieces = Infinity::Board::GeneratePieces(seed,width,height,values);
		Infinity::Board * board = new Infinity::Board(width,height);
		MajorTest(pieces, board, threadCount);
	}

	if(true)
	{
		std::cout << "Here we go..." << std::endl;
		Infinity::Pieces * eternityPieces = new Infinity::Pieces;
		Infinity::Board * eternityBoard = new Infinity::Board(16,16);
		Infinity::Piece * piece = new Infinity::Piece(139, 0x01, Infinity::Yellow_LBlue_Star, Infinity::Purple_Yellow_Circle, Infinity::Purple_Yellow_Circle, Infinity::Green_Orange_Diamond);
		eternityBoard->place(piece, 8, 7);
		eternityBoard->debugOutput();
		Infinity::Eternity(eternityPieces, seed);
		Infinity::PiecesAnalyse(eternityPieces, 22);
		MajorTest(eternityPieces, eternityBoard, threadCount);	
	}


#ifdef _DEBUG
#ifndef AUTOMATIC_MEMORY
	_CrtDumpMemoryLeaks();
#endif // AUTOMATIC_MEMORY
#endif //_DEBUG

	return EXIT_SUCCESS;
}

void SetPieces(Infinity::Pieces * pieces_)
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

void SaveTest(void)
{
	Infinity::Pieces * savePieces = new Infinity::Pieces;
	SetPieces(savePieces);
	
	Infinity::Board * saveBoard = new Infinity::Board(4,4, 3, 3);

	Infinity::Pieces::iterator i  = savePieces->begin();

	saveBoard->place(*i++, 0, 0);
	saveBoard->place(*i++, 0, 1);
	saveBoard->place(*i++, 1, 0);
	saveBoard->place(*i++, 1, 1);
	saveBoard->place(*i++, 1, 2);
	saveBoard->place(*i++, 2, 1);
	saveBoard->place(*i++, 2, 2);
	saveBoard->place(*i++, 2, 3);
	saveBoard->place(*i++, 3, 2);

	Infinity::Solver * saveSolver = new Infinity::Solver(saveBoard, savePieces, 1);

	{
		std::ofstream saveFile;
		saveFile.open("saveFile.dat", std::ios::binary | std::ios::out);
		
		SavePieces(saveFile, *savePieces);
		saveBoard->save(saveFile);
		saveSolver->save(saveFile);
		int solver = 0;
		saveFile.write(reinterpret_cast<const char *>(&solver), sizeof(int)); // No more solvers
	}

	Infinity::Pieces * loadPieces = new Infinity::Pieces;
	Infinity::Board * loadBoard = 0;
	Infinity::Solver * loadSolver = 0;

	{
		std::ifstream loadFile;
		loadFile.open("saveFile.dat", std::ios::binary | std::ios::in);

		LoadPieces(loadFile, *loadPieces);
		loadBoard = new Infinity::Board(loadFile, *loadPieces);

		std::list<Infinity::Solver *> solvers = Infinity::LoadSolvers(loadFile, loadBoard, loadPieces);
		assert(solvers.size() == 1);
		loadSolver = (*(solvers.begin()));
	}

	assert(savePieces->size() == loadPieces->size());

	Infinity::Pieces::iterator j = loadPieces->begin();
	for (Infinity::Pieces::iterator i = savePieces->begin(); i != savePieces->end(); ++i)
	{
		assert( (*i)->id() == (*j)->id() );
		assert( (*i)->rotation() == (*j)->rotation() );
		assert( (*i)->matches(*j) );
		++j;
	}

	assert(saveBoard->width() == loadBoard->width());
	assert(saveBoard->height() == loadBoard->height());
	assert(saveBoard->startX() == loadBoard->startX());
	assert(saveBoard->startY() == loadBoard->startY());

	for (Infinity::CooOrd i = 0; i < saveBoard->width(); ++i)
	{
		for (Infinity::CooOrd j = 0; j < saveBoard->height(); ++j)
		{
			if(saveBoard->getPiece(i, j))
			{
				assert(saveBoard->getPiece(i, j)->id() == loadBoard->getPiece(i, j)->id());
			}
			else
			{
				assert(!loadBoard->getPiece(i, j));
			}
		}
	}

	assert(saveSolver->runPieces()->size() == loadSolver->runPieces()->size());
	assert(saveSolver->currentPieceIndex() == loadSolver->currentPieceIndex());


	for (Infinity::Pieces::iterator i = savePieces->begin(); i !=savePieces->end(); ++i)
	{
		delete *i;
	}

	for (Infinity::Pieces::iterator i = loadPieces->begin(); i !=loadPieces->end(); ++i)
	{
		delete *i;
	}
	delete loadPieces;

	delete loadBoard;
	delete saveSolver;
	delete loadSolver;
}

void Solve(AutoMatic::JobManager & jobManager, Infinity::Pieces * pieces_, Infinity::Board * board_)
{
	clock_t start = Infinity::Solver::_DebugStartTime = clock();
	
	jobManager.attach();

	//char c = 0;
	//std::cin >> c;

	//if(c == 's')
	if(false)
	{
		std::cout << "Saving" << std::endl;
		Infinity::Solver::_DebugSaveFile.open("debugSaveFile.dat");

		SavePieces(Infinity::Solver::_DebugSaveFile, *pieces_);
		board_->save(Infinity::Solver::_DebugSaveFile);
		Infinity::Solver::_DebugSaveFile.flush();
		Infinity::Solver::_DebugSave = true;
		jobManager.waitUntilTerminated();

		int solver = 0;
		Infinity::Solver::_DebugSaveFile.write(reinterpret_cast<const char *>(&solver), sizeof(int)); // No more solvers

		std::cout << "Saved " << Infinity::_DebugSavedCount << " solvers!" << std::endl;
	}
	else
	{
		jobManager.waitUntilTerminated();
	}

	clock_t stop = clock();

	std::cout << "Max pending jobs: " << jobManager._debugMaxPendingJobs << std::endl;
	std::cout << "Solutions: " << Infinity::DebugOutput::Solutions() << std::endl;
	std::cout << "Time: " << stop - start << std::endl;
	std::cout << "Process Usage:\n";
	for (std::map<int,int>::iterator i = AutoMatic::Process::_DebugProcessUsage.begin();
		i != AutoMatic::Process::_DebugProcessUsage.end(); ++i)
	{
		std::cout << (*i).first << ": " << (*i).second << std::endl;
	}

	std::cout << std::endl;
}

void MajorTest(Infinity::Pieces * pieces_, Infinity::Board * board_, int threads_)
{
	Infinity::DebugOutput::ResetSolutions();
	Infinity::Solver::_SolutionFound = false;
	AutoMatic::Process::_DebugId = 0;

	std::cout << "Starting major test... " << std::endl;
	
	Infinity::Pieces * delPieces = 0;
	{
		AutoMatic::JobManager jobManager;
		AutoMatic::ThreadProcessManager threadProcessManager(&jobManager, threads_);

		{
			delPieces = new Infinity::Pieces(*pieces_);

			Infinity::Solver * solve = new Infinity::Solver(board_, pieces_, 1);
			jobManager._debugMaxProcesses = threads_ / 2;
			jobManager.queueJob(solve);
			Solve(jobManager, pieces_, board_);
		}
	}

	for (Infinity::Pieces::iterator i = delPieces->begin(); i !=delPieces->end(); ++i)
	{
		delete *i;
	}
	delete delPieces;
}

void LoadFile(std::ifstream & loadFile, int threads_)
{
	Infinity::Pieces * loadPieces = new Infinity::Pieces;

	LoadPieces(loadFile, *loadPieces);
	Infinity::Board * loadBoard = new Infinity::Board(loadFile, *loadPieces);

	std::cout << "Loaded " << loadPieces->size() << " pieces. " << std::endl;
	std::cout << "Board: W: " << loadBoard->width() << " H: " << loadBoard->height() << std::endl;

	std::list<Infinity::Solver *> solvers = Infinity::LoadSolvers(loadFile, loadBoard, loadPieces);

	AutoMatic::JobManager jobManager;
	AutoMatic::ThreadProcessManager threadProcessManager(&jobManager, threads_);

	jobManager._debugMaxProcesses = threads_ / 2;
	
	for (std::list<Infinity::Solver *>::iterator i = solvers.begin(); i != solvers.end(); ++i)
	{
		jobManager.queueJob(*i);
	}

	std::cout << "Loaded " << solvers.size() << " solvers!" << std::endl;

	if(solvers.size() > 0)
	{
		Solve(jobManager, loadPieces, loadBoard);
	}

	for (Infinity::Pieces::iterator i = loadPieces->begin(); i !=loadPieces->end(); ++i)
	{
		delete *i;
	}
	delete loadPieces;
	delete loadBoard;
}
