#include <cassert>
#include <exception>

#include "AM_Job.hpp"
#include "AM_ProcessManager.hpp"
#include "AM_Process.hpp"

namespace AutoMatic
{
	Process::Process(ProcessManager * processManager_)
		:_completedJob(0), _job(0)
	{
		link(processManager_);
	}

	void Process::link(ProcessManager * processManager_)
	{
		_processManagers.push_back(processManager_);
	}

	void Process::queueJob(Job * job_)
	{ 
		for (std::vector<ProcessManager *>::iterator i = _processManagers.begin();
			i != _processManagers.end(); ++i)
		{
			(*i)->queueJob(job_); 
		}		
	}

	Job * Process::completedJob(void) 
	{ 
		Job * job = _completedJob;
		_completedJob = 0;
		return job; 
	}

	void Process::receiveJob(Job	* job_)
	{ 
		_completedJob = 0;
		_job = job_;	

		for (std::vector<ProcessManager *>::iterator i = _processManagers.begin();
			i != _processManagers.end(); ++i)
		{
			(*i)->signalJobTaken(this);
		}
	}

	void Process::runJob(void)
	{
		Job * nextJob = _job->run(this);
		_completedJob = _job;			
		_job = nextJob;			

		for (std::vector<ProcessManager *>::iterator i = _processManagers.begin();
			i != _processManagers.end(); ++i)
		{
			(*i)->signalJobFinished(this);
		}
	}
}
