#include <cassert>

#include "AM_Job.hpp"
#include "AM_Process.hpp"
#include "AM_ProcessManager.hpp"
#include "AM_JobManager.hpp"

namespace AutoMatic
{
	JobManager::JobManager(void)
		: _debugMaxPendingJobs(0), _jobsInProcess(0)
	{}

	JobManager::~JobManager(void)
	{
		while(_pendingJobs.size() > 0)
		{
			delete _pendingJobs.front();			
			_pendingJobs.pop();
		}
	}

	void JobManager::addProcessManager(ProcessManager * processManager)
	{
		_processManagers.push_back(processManager);
	}

	bool JobManager::update(void)
	{
		for (std::vector<ProcessManager *>::iterator i = _processManagers.begin();
			i != _processManagers.end(); ++i)
		{
			(*i)->update();
		}

		if(_pendingJobs.size() > _debugMaxPendingJobs)
		{ 
			_debugMaxPendingJobs = _pendingJobs.size(); 
		}

		return (_jobsInProcess == 0);
	}

	void JobManager::addProcess(Process * process_)
	{ 
		if(Job * job = process_->completedJob()){	--_jobsInProcess; delete job;	}

		if(!sendPendingJobToProcess(process_))
		{
			_availableProcesses.insert(process_);
		}
	}

	void JobManager::remProcess(Process * process_)
	{ 
		_availableProcesses.erase(process_); 
		if(Job * job = process_->completedJob()){ --_jobsInProcess; delete job; }
		if(Job * job = process_->job()){ --_jobsInProcess; queueJob(job); }
	}

	void JobManager::queueJob(Job * job_)
	{ 
		++_jobsInProcess;
		if(!sendJobToAvailableProcess(job_))
		{
			_pendingJobs.push(job_); 
		}
	}

	void JobManager::queueJob(char jobFactoryId_, std::string const & job_)
	{
		JobFactory * jobFactory = _jobFactories[jobFactoryId_];
		if(jobFactory)
		{
			Job * job = jobFactory->create(job_);
			if(job)
			{
				queueJob(job);
			}
		}
	}

	void JobManager::addJobFactory(JobFactory * jobFactory_)
	{
		_jobFactories[jobFactory_->id()] = jobFactory_;
	}

	JobFactory * JobManager::getJobFactory(char jobFactoryId_)
	{ 
		return _jobFactories[jobFactoryId_]; 
	}

	void JobManager::processJob(Process * process_, Job * job_)
	{
		process_->receiveJob(job_);
	}

	bool JobManager::sendJobToAvailableProcess(Job * job_)
	{
		if(0 == _availableProcesses.size()){ return false; }
		Process * process = *_availableProcesses.begin();
		_availableProcesses.erase(_availableProcesses.begin());
		processJob(process, job_);
		return true;
	}

	bool JobManager::sendPendingJobToProcess(Process * process_)
	{
		if(0 == _pendingJobs.size()){ return false; }
		Job * job = _pendingJobs.front();
		_pendingJobs.pop();
		processJob(process_, job);
		return true;
	}
}
