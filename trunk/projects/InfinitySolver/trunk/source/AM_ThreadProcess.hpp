#ifndef _AM_ThreadProcess_hpp_
#define _AM_ThreadProcess_hpp_

#include "AM_MessageQueue.hpp"
#include "AM_Threading.hpp"
#include "AM_Process.hpp"

namespace AutoMatic
{
	class ThreadProcessManager;
	class ThreadProcess : public Process, public Thread
	{
	public:
		ThreadProcess(ProcessManager * processManager_);

		virtual void receiveJob(Job	* job_);
		virtual void update(void);

		virtual char const id(void) const { return 0x01; }

		virtual void requestPause(void);

	private:
		MessageQueue<Job *> _jobs;
	};
}

#endif // _AM_ThreadProcess_hpp_