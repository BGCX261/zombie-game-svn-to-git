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

	protected:
		JobManager * _jobManager;
	};
}

#endif // _AM_ProcessManager_hpp_