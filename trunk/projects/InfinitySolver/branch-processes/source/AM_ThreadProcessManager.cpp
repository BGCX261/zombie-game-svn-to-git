#include <iostream>

#include "AM_Job.hpp"
#include "AM_JobManager.hpp"

#include "AM_ThreadProcess.hpp"
#include "AM_ThreadProcessManager.hpp"

#include "AM_Memory.h"

namespace AutoMatic
{
	ThreadProcessManager::ThreadProcessManager(JobManager * jobManager_, int threadProcessCount_)
		: ProcessManager(jobManager_), _threadProcessCount(threadProcessCount_)
	{
		_threadProcesses = new ThreadProcess * [threadProcessCount_];
		for (int i = 0; i < _threadProcessCount; ++i)
		{
			_threadProcesses[i] = new ThreadProcess(jobManager_);
			_jobManager->addProcess(_threadProcesses[i]);
		}		

	}

	ThreadProcessManager::~ThreadProcessManager(void)
	{
		for (int i = 0; i < _threadProcessCount; ++i)
		{
			_jobManager->remProcess(_threadProcesses[i]);
			delete _threadProcesses[i];
			_threadProcesses[i] = 0;
		}

		delete[] _threadProcesses; 
		_threadProcesses = 0;
	}
}
