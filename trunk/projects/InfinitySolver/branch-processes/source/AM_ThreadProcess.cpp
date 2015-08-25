#include <cassert>

#include "AM_Job.hpp"
#include "AM_ProcessManager.hpp"

#include "Infinity_Board.hpp"

#include "AM_ThreadProcess.hpp"

#include "AM_Memory.h"

namespace AutoMatic
{
	ThreadProcess::ThreadProcess(JobManager * jobManager_) 
		: Process(jobManager_)
	{
		attach();
	}

	ThreadProcess::~ThreadProcess(void)
	{
		requestPause();
		_jobs.cancelWait();
		terminate();
	}

	void ThreadProcess::receiveJob(Job * job_)
	{
		_jobs.push(job_);
	}

	void ThreadProcess::update(void)
	{
		_jobs.waitForMessage();
		
		MessageQueue<Job *>::Queue * jobs = _jobs.get();
		if(jobs->size() == 0){ return; } // TODO: I don't know how this is possible.
		assert(jobs && jobs->size() == 1);

		Process::receiveJob(jobs->front());
		Process::runJob();
		jobs->pop();
	}

	void ThreadProcess::requestPause(void)
	{
		Thread::requestPause();
		_jobs.cancelWait();
	}
}
