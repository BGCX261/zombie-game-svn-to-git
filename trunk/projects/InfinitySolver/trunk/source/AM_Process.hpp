#ifndef _AM_Process_hpp_
#define _AM_Process_hpp_

#include <vector>

namespace AutoMatic
{
	class Job;
	class ProcessManager;
	class Process
	{
	public:
		Process(ProcessManager * processManager_);

		virtual ~Process(void){}

		virtual void link(ProcessManager * processManager_);

		/// 
		/** Receive a job to run.
		*** Signal each ProcessManager that a job has been taken.
		*/
		virtual void receiveJob(Job	* job_);

		/// 
		/** Send a job to the ProcessManager.
		***
		*/
		virtual void queueJob(Job * job_);

		Job * completedJob(void);

		Job * job(void) const { return _job; }

		/// 
		/** Run the current job.   
		*** Signal each ProcessManager when the job is completed.
		*/
		virtual	void runJob(void);

		/// 
		/** Get the id of this process.
		*** This id should be unique between each derived Process class.
		*/
		virtual char const id(void) const = 0;

	protected:
		std::vector<ProcessManager *> _processManagers;
		Job * _completedJob;
		Job	* _job;
	};
}

#endif // _AM_Process_hpp_