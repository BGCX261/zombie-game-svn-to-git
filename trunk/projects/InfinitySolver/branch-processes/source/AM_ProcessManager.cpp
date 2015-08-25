#include "AM_JobManager.hpp"
#include "AM_ProcessManager.hpp"

#include "AM_Memory.h"

namespace AutoMatic
{
	ProcessManager::ProcessManager(JobManager * jobManager_)
		: _jobManager(jobManager_)
	{
	}
}
