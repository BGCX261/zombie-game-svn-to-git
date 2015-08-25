#ifndef _AM_NetworkProcessManager_hpp_
#define _AM_NetworkProcessManager_hpp_

#include <set>
#include <list>
#include <queue>
#include <enet/enet.h>
#include "AM_MessageQueue.hpp"
#include "AM_ProcessManager.hpp"

namespace AutoMatic
{
	class NetworkProcess;

	/// 
	/** Used to maintain local information on remote processes.
	*** Keeps track of Jobs sent off to remote processes, if those jobs
		* aren't completed, they are sent back to the local process.
		* It also informs remote job managers of local processes.
		* 
		* What if the process completes a job, and fails the next job (created from
		* the original)?
	*/
	class NetworkProcessManager : public ProcessManager
	{
	public:
		enum Channels { New_Process = 0, New_Job, Process_Unavailable, All_Channels };

		NetworkProcessManager(JobManager * jobManager_, enet_uint16 port_);
		~NetworkProcessManager(void);

		/// Connect to a system.
		bool connect(char const * address_, enet_uint16 port_);

		virtual void update(void);

		virtual void queueJob(Job * job_);

		virtual void signalJobFinished(Process * process_);

		virtual void signalJobTaken(Process * process_);

		void sendJob(Job * job_, ENetPeer * peer_);

	private:

		static int _ENetInitialised;

		NetworkProcess * createNetworkProcess(ENetPeer * peer_);
		void destroyNetworkProcess(NetworkProcess * networkProcess_);

		std::list<NetworkProcess *> _networkProcesses;
		std::set<ENetPeer *> _peers;

		ENetHost * _server;

		MessageQueue<Channels> _messages;
	};
}

#endif // _AM_NetworkProcessManager_hpp_