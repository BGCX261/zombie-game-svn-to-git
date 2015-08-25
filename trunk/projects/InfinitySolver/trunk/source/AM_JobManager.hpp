#ifndef _AM_LocalJobManager_hpp_
#define _AM_LocalJobManager_hpp_

#include <set>
#include <map>
#include <queue>
#include <vector>

namespace AutoMatic
{
	class Job;
	class JobFactory;

	class Process;
	class ProcessManager;

	/// 
	/** The JobManager stores Processes and Jobs, and dispatches Jobs when there
	*** are available Processes.
	*/
	class JobManager
	{
	public:
		JobManager(void);
		~JobManager(void);

		/// Add a ProcessManager, to be updated when update() is called.
		void addProcessManager(ProcessManager * processManager);

		/// Update this JobManager
		/** Call update() for each processManager and check if and Job's remain
			* to be processes / in process.
			* \return true if no jobs remain to be processed.
		*/
		bool update(void);

		/// Store a process, to be made available to any pending jobs.
		void addProcess(Process * process_);
		/// Remove a process from the process pool.
		void remProcess(Process * process_);

		/// Queue a job to be processed by any available process.
		void queueJob(Job * job_);
		/// Queue a job, after de-serialising the string using the appropriate factory.
		void queueJob(char jobFactoryId_, std::string const & job_);

		/// Add a job factory to be used when de-serialising a job.
		void addJobFactory(JobFactory * jobFactory_);
		/// Get a job factory with the specified id.
		JobFactory * getJobFactory(char jobFactoryId_);

	private:
		void processJob(Process * process_, Job * job_);
		bool sendJobToAvailableProcess(Job * job_);
		bool sendPendingJobToProcess(Process * process_);

		int _jobsInProcess;
		std::queue<Job *> _pendingJobs;
		
		std::set<Process *> _availableProcesses;
		
		std::vector<ProcessManager *> _processManagers;

		std::map<char, JobFactory *> _jobFactories;

	public:
		std::queue<Job *>::size_type _debugMaxPendingJobs;
	};
}

#endif // _AM_LocalJobManager_hpp_