#ifndef _AM_ThreadProcessManager_hpp_
#define _AM_ThreadProcessManager_hpp_

#include <queue>
#include "AM_MessageQueue.hpp"
#include "AM_ProcessManager.hpp"

namespace AutoMatic
{
	class ThreadProcess;
	class ThreadProcessManager : public ProcessManager
	{
	public:
		ThreadProcessManager(JobManager * jobManager_, int threadProcessCount_);
		virtual ~ThreadProcessManager(void);

		virtual void queueJob(Job * job_);
		virtual void update(void);

		virtual void signalJobFinished(Process * process_);
	
		virtual void link(ProcessManager * processManager_);

	private:

		void sendPendingJobsToJobManager(void);
		void sendAvailableProcessesToJobManager(void);

		int _threadProcessCount;
		ThreadProcess ** _threadProcesses;
		MessageQueue<Job *> _jobs;
		MessageQueue<Process *> _processes;
	};
}

#endif // _AM_ThreadProcessManager_hpp_