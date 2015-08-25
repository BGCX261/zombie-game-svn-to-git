#include "AM_Job.hpp"
#include "AM_JobManager.hpp"

#include "AM_ThreadProcess.hpp"
#include "AM_ThreadProcessManager.hpp"

namespace AutoMatic
{
	ThreadProcessManager::ThreadProcessManager(JobManager * jobManager_, int threadProcessCount_)
		: ProcessManager(jobManager_), _threadProcessCount(threadProcessCount_)
	{
		_threadProcesses = new ThreadProcess * [threadProcessCount_];
		for (int i = 0; i < _threadProcessCount; ++i)
		{
			_threadProcesses[i] = new ThreadProcess(this);
			_jobManager->addProcess(_threadProcesses[i]);
		}		
	}

	ThreadProcessManager::~ThreadProcessManager(void)
	{
		for (int i = 0; i < _threadProcessCount; ++i)
		{
			_jobManager->remProcess(_threadProcesses[i]);
			_threadProcesses[i]->requestPause();
			_threadProcesses[i]->waitUntilPaused();
			delete _threadProcesses[i];
		}

		delete[] _threadProcesses;

		MessageQueue<Job *>::Queue * jobs = _jobs.get();

		while(!jobs->empty())
		{
			delete jobs->front();
			jobs->pop();
		}

		delete jobs;
	}

	void ThreadProcessManager::queueJob(Job * job_)
	{
		_jobs.push(job_);
	}

	void ThreadProcessManager::signalJobFinished(Process * process_)
	{
		_processes.push(process_);
	}

	void ThreadProcessManager::link(ProcessManager * processManager_)
	{
		for (int i = 0; i < _threadProcessCount; ++i)
		{
			_threadProcesses[i]->requestPause();
			_threadProcesses[i]->waitUntilPaused();
			_threadProcesses[i]->link(processManager_);
			_threadProcesses[i]->unpause();
		}
	}

	void ThreadProcessManager::update(void)
	{
		sendAvailableProcessesToJobManager();
		sendPendingJobsToJobManager();
	}

	void ThreadProcessManager::sendPendingJobsToJobManager(void)
	{
		MessageQueue<Job *>::Queue * jobs = _jobs.get();

		while(!jobs->empty())
		{
			Job * job = jobs->front();
			jobs->pop();
			_jobManager->queueJob(job);
		}		

		delete jobs;
	}

	void ThreadProcessManager::sendAvailableProcessesToJobManager(void)
	{
		MessageQueue<Process *>::Queue * processes = _processes.get();
		
		while (!processes->empty())
		{
			Process * process = processes->front();
			processes->pop();
			_jobManager->addProcess(process);
		}

		delete processes;
	}
}
