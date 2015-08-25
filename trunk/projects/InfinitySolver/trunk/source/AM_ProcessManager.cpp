#include "AM_JobManager.hpp"
#include "AM_ProcessManager.hpp"

namespace AutoMatic
{
	ProcessManager::ProcessManager(JobManager * jobManager_)
		: _jobManager(jobManager_)
	{
		_jobManager->addProcessManager(this);
	}

	void ProcessManager::queueJob(Job * job_)
	{ 
		_jobManager->queueJob(job_); 
	}

	void ProcessManager::signalJobFinished(Process * )
	{
		
	}

	void ProcessManager::signalJobTaken(Process * )
	{

	}
}
