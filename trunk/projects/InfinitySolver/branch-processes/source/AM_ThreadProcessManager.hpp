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

	private:
		int _threadProcessCount; /**< The number of ThreadProcesses this ThreadManager has. **/
		ThreadProcess ** _threadProcesses; /**< The ThreadProcesses created by this ThreadProcessManager. **/
	};
}

#endif // _AM_ThreadProcessManager_hpp_