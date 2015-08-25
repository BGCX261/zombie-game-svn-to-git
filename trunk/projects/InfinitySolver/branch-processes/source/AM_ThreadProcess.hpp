#ifndef _AM_ThreadProcess_hpp_
#define _AM_ThreadProcess_hpp_

#include "AM_MessageQueue.hpp"
#include "AM_Threading.hpp"
#include "AM_Process.hpp"

namespace AutoMatic
{
	class ThreadProcess : public Process, public Thread
	{
	public:
		ThreadProcess(JobManager * jobManager_);
		virtual ~ThreadProcess(void);

		/// Receive a job from the JobManager.
		/** Adds the job to the pending jobs.
		*** Should never receive more than one at a time.
		*/
		virtual void receiveJob(Job	* job_);

		/// Update the ThreadProcess, wait on a new job and perform it.
		/**
		***
		*/
		virtual void update(void);

		virtual void requestPause(void);

	private:
		MessageQueue<Job *> _jobs;
	};
}

#endif // _AM_ThreadProcess_hpp_