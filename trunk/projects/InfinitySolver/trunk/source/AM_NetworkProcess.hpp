#ifndef _AM_NetworkProcess_hpp_
#define _AM_NetworkProcess_hpp_

#include <enet/enet.h>
#include "AM_Process.hpp"

namespace AutoMatic
{
	class NetworkProcessManager;

	static const char NetworkProcessId = 0x02;

	/// 
	/** The NetworkProcess provides access to a remote process.
	***
	*/
	class NetworkProcess : public Process
	{
	public:
		NetworkProcess(ENetPeer * peer_, NetworkProcessManager * networkProcessManager_);

		bool isPeer(ENetPeer const * peer_) const { return _peer == peer_; }

		/// Send the job to the remote system.
		virtual void receiveJob(Job	* job_);
		/// Does nothing.  Should never be called.
		virtual void queueJob(Job * job_);
		/// Does nothing.  Should never be called.
		virtual	void runJob(void);

		virtual char const id(void) const { return NetworkProcessId; }

	private:
		ENetPeer * _peer;
		NetworkProcessManager * _networkProcessManager;
	};
}

#endif //_AM_NetworkProcess_hpp_