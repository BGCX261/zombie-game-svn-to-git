#include <cassert>

#include "AM_Job.hpp"
#include "AM_ProcessManager.hpp"

#include "Infinity_Board.hpp"

#include "AM_ThreadProcess.hpp"

namespace AutoMatic
{
	ThreadProcess::ThreadProcess(ProcessManager * processManager_) 
		: Process(processManager_)
	{
		attach();
	}

	void ThreadProcess::receiveJob(Job * job_)
	{
		_jobs.push(job_);
	}

	void ThreadProcess::update(void)
	{
		if(_job)
		{
			_job = _job->run(this);
		}
		else
		{
			_jobs.waitForMessage();

			MessageQueue<Job *>::Queue * jobs = _jobs.get();
			while (jobs && !jobs->empty())
			{
				Process::receiveJob(jobs->front());
				Process::runJob();
				jobs->pop();
			}		
		}
	}

	void ThreadProcess::requestPause(void)
	{
		Thread::requestPause();
		_jobs.cancelWait();
	}
}
