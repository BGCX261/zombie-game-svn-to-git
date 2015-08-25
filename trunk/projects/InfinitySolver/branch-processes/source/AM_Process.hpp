#ifndef _AM_Process_hpp_
#define _AM_Process_hpp_

#include <map>
#include <queue>
#include <vector>

#include <boost/thread/recursive_mutex.hpp>

#include "AM_Job.hpp"

namespace AutoMatic
{
	class JobManager;
	class Process
	{
	public:
		Process(JobManager * jobManager_);

		virtual ~Process(void)
		{

		}

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

		/// Returns a pointer to a completed job, if one has been completed.
		/** Also null's the _completedJob pointer
		***
		*/
		Job * completedJob(void);

		Job * job(void) const { return _job; }

		/// 
		/** Run the current job.   
		*** Signal each ProcessManager when the job is completed.
		*/
		virtual	void runJob(void);

	protected:
		std::priority_queue<Job *, std::vector<Job *>, JobPriority > _localJobs; /**< Jobs created through running the received job. **/
		JobManager * _jobManager;

		int _queueJobCount; /**< The number of jobs to queue (send back to the JobManager).  All others will be localJobs. **/

		Job * _completedJob; /**< Holds a pointer to a completed job, that was received. **/
		Job	* _job; /**< Holds a pointer to a received job. **/

	public:
		int _debugId;
		static int _DebugId;
		static std::map<int, int> _DebugProcessUsage;
	};
}

#endif // _AM_Process_hpp_