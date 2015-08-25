#include <cassert>
#include "AM_NetworkProcessManager.hpp"
#include "AM_NetworkProcess.hpp"

namespace AutoMatic
{
	NetworkProcess::NetworkProcess(ENetPeer * peer_, NetworkProcessManager * networkProcessManager_)
		: Process(networkProcessManager_), _networkProcessManager(networkProcessManager_), _peer(peer_)
	{}

	void NetworkProcess::receiveJob(Job	* job_)
	{
		_networkProcessManager->sendJob(job_, _peer);
		Process::receiveJob(job_);
	}

	void NetworkProcess::queueJob(Job * )
	{
		assert(false);
	}

	void NetworkProcess::runJob(void)
	{
		assert(false);
	}
}
