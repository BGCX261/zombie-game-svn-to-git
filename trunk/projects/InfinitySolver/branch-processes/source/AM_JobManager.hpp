#ifndef _AM_LocalJobManager_hpp_
#define _AM_LocalJobManager_hpp_

#include <set>
#include <map>
#include <queue>
#include <vector>

#include "AM_Job.hpp"
#include "AM_Threading.hpp"
#include "AM_MessageQueue.hpp"

namespace AutoMatic
{
	class Job;
	class Process;
	class ProcessManager;

	/// 
	/** The JobManager stores Processes and Jobs, and dispatches Jobs when there
	*** are available Processes.
	*/
	class JobManager : public Thread
	{
	public:
		JobManager(void);
		~JobManager(void);

		/// Update this JobManager
		/** Call update() for each processManager and check if and Job's remain
			* to be processes / in process.
			* \return true if no jobs remain to be processed.
		*/
		void update(void);

		/// Store a process, to be made available to any pending jobs.
		void addProcess(Process * process_);
		/// Remove a process from the process pool.
		void remProcess(Process * process_);

		/// Queue a job to be processed by any available process.
		void queueJob(Job * job_);

		/// Send a Process Message to this JobManager.
		void sendProcessMessage(Process * process_);

		/// Send a Job Message to this JobManager.
		void sendJobMessage(Job * job_);
	
		/// Is a job required by this job manager?
		bool isJobNeeded(void);

	private:
		void setIsJobNeeded(bool isJobNeeded_);

		/// Send the \a process_ a \a job_.
		void processJob(Process * process_, Job * job_);

		/// Send a Job to any available process. 
		/** \return false if no available processes.
		*/
		bool sendJobToAvailableProcess(Job * job_);

		/// Send a Job to the \a process_.
		/** \return false if no available jobs.
		*/
		bool sendPendingJobToProcess(Process * process_);

		MessageQueue< std::pair<Process *, Job *> > _processAndJobQueue; /**< Process and Job messages, waiting to be read. **/

		int _jobsInProcess; /**< The number of jobs currently being processed. **/

		std::priority_queue<Job *, std::vector<Job *>, JobPriority > _pendingJobs; /**< The Jobs waiting to be processed. **/
		
		std::set<Process *> _availableProcesses; /**< Processes available to take jobs. **/
		
		std::pair<boost::recursive_mutex, bool> _isJobNeeded;

	public:
		int _debugMaxProcesses;
		std::queue<Job *>::size_type _debugMaxPendingJobs;
	};
}

#endif // _AM_LocalJobManager_hpp_