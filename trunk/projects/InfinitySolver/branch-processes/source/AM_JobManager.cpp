#include <cassert>
#include <iostream>

#include "AM_Process.hpp"
#include "AM_ProcessManager.hpp"
#include "AM_JobManager.hpp"

#include "AM_Memory.h"

namespace AutoMatic
{
	JobManager::JobManager(void)
		: _debugMaxPendingJobs(0), _jobsInProcess(0), _debugMaxProcesses(0)
	{
		_isJobNeeded.second = true;
	}

	JobManager::~JobManager(void)
	{
		while(_pendingJobs.size() > 0)
		{
			delete _pendingJobs.top();			
			_pendingJobs.pop();
		}
	}

	void JobManager::update(void)
	{
		if(_pendingJobs.size() > _debugMaxPendingJobs)
		{ 
			_debugMaxPendingJobs = _pendingJobs.size(); 
		}

		{
			if(_pendingJobs.size() == 0 &&_jobsInProcess == 0){ setTerminate(true); }
			else { _processAndJobQueue.waitForMessage(); }
			MessageQueue< std::pair<Process *, Job *> >::Queue * processesAndJobs = _processAndJobQueue.get();
			while(processesAndJobs && !processesAndJobs->empty())
			{
				if( processesAndJobs->front().first )
				{
					addProcess(processesAndJobs->front().first);
				}
				else if( processesAndJobs->front().second )
				{
					queueJob(processesAndJobs->front().second);
				}

				processesAndJobs->pop();
			}

			setIsJobNeeded((_pendingJobs.size() < _debugMaxProcesses));
			
			delete processesAndJobs;
		}		
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

	void JobManager::sendProcessMessage(Process * process_)
	{
		_processAndJobQueue.push(std::pair<Process *, Job *>(process_, 0));
	}

	void JobManager::sendJobMessage(Job * job_)
	{
		_processAndJobQueue.push(std::pair<Process *, Job *>(0, job_));
	}

	bool JobManager::isJobNeeded(void)
	{
		boost::recursive_mutex::scoped_lock isJobNeededLock(_isJobNeeded.first);
		return _isJobNeeded.second;
	}

	void JobManager::setIsJobNeeded(bool isJobNeeded_)
	{
		boost::recursive_mutex::scoped_lock debugIsJobNeededLock(_isJobNeeded.first);
		_isJobNeeded.second = isJobNeeded_;
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
		Job * job = _pendingJobs.top();
		_pendingJobs.pop();
		processJob(process_, job);
		return true;
	}
}
