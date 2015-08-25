#include <cassert>
#include <exception>

#include "AM_JobManager.hpp"
#include "AM_Process.hpp"

#include "AM_Memory.h"

namespace AutoMatic
{
	int const DebugQueueJobCount = 2; // TODO: Find out the best number of jobs reserve.

	int Process::_DebugId = 0;
	std::map<int, int> Process::_DebugProcessUsage;

	Process::Process(JobManager * jobManager_)
		: _completedJob(0), _job(0)
		, _queueJobCount(DebugQueueJobCount)
		, _jobManager(jobManager_)
	{
		_debugId = _DebugId++;
		_DebugProcessUsage[_debugId] = 0;
	}

	void Process::queueJob(Job * job_)
	{ 
		// Null the parent, because the parent's other children will be taken care of, and it will be deleted.
		job_->parent(0);

		// Send back the required number of jobs to the job manager,
		bool sentJobBack = false;

		if(_queueJobCount == 0)
		{ // It will only get to here if it has pushed enough local jobs onto the queue.
			if(_jobManager->isJobNeeded()){ _jobManager->sendJobMessage(job_); sentJobBack = true; }
			else { _queueJobCount--; }
		}
		
		if(!sentJobBack)
		{ // Did not send the job to the manager (either doesn't need it, or needs it here).
			_localJobs.push(job_); 
			if(_queueJobCount > 0){ _queueJobCount--; }
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
	}

	// 1. Queue up DebugQueueJobCount of Jobs.
	// 2. Try to send jobs to the JobManager, until it doesn't need them.
	// 3. Solved the rest of the jobs in this process, with DFS

	void Process::runJob(void)
	{
		Job * nextJob = 0;
		_queueJobCount = DebugQueueJobCount;
		
		while(true)
		{					
			while (_queueJobCount > -1 || _localJobs.size() < DebugQueueJobCount)
			{ // _queueJobCount will be > -1 while the JobManager hasn't returned false for job-needed.
				_DebugProcessUsage[_debugId]++;
				nextJob = _job->run(this);
				if(!nextJob){ break; }
				queueJob(nextJob); // Send it back to the JobManager, or add it to the local queue.
			}

			if(_localJobs.empty()){ break; }
			
			Job * job = _localJobs.top();
			_localJobs.pop();

			assert(!job->parent());

			do 
			{ // Perform a DFS until all paths have been exhausted.
				_DebugProcessUsage[_debugId]++;
				nextJob = job->run(this);
				if(!nextJob)
				{ // Job has no more children, return to the parent.
					nextJob = job->parent();
					delete job; 
				}

				job = nextJob;
			} while(job);
		}

		_completedJob = _job;	
		_job = 0;	

		// Return this process to the JobManager.
		_jobManager->sendProcessMessage(this);
	}
}
