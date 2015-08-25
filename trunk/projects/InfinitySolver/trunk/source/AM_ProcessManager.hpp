#ifndef _AM_ProcessManager_hpp_
#define _AM_ProcessManager_hpp_

namespace AutoMatic
{
	class Job;
	class Process;
	class JobManager;
	class ProcessManager
	{
	public:
		ProcessManager(JobManager * jobManager_);

		virtual ~ProcessManager(void){}

		virtual void update(void){ }

		virtual void queueJob(Job * job_);

		virtual void signalJobFinished(Process * process_);

		virtual void signalJobTaken(Process * process_);

		virtual void link(ProcessManager * ){ }

	protected:
		JobManager * _jobManager;
	};
}

#endif // _AM_ProcessManager_hpp_