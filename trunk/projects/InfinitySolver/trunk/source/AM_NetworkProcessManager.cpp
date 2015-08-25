#include "AM_Job.hpp"
#include "AM_JobManager.hpp"
#include "AM_NetworkProcess.hpp"
#include "AM_NetworkProcessManager.hpp"

namespace AutoMatic
{
	int NetworkProcessManager::_ENetInitialised = 0;

	NetworkProcessManager::NetworkProcessManager(JobManager * jobManager_, enet_uint16 port_)
		: ProcessManager(jobManager_), _server(0)
	{
		if(0 == _ENetInitialised++){ enet_initialize(); }
		ENetAddress address;
		
		address.host = ENET_HOST_ANY;
		address.port = port_;

		_server = enet_host_create (& address, 1000, 0,	0);
		if (!_server){ throw std::exception("Could not create host"); }
	}

	NetworkProcessManager::~NetworkProcessManager(void)
	{
		enet_host_destroy(_server);
		if(0 == --_ENetInitialised){ enet_deinitialize(); }
	}

	bool NetworkProcessManager::connect(char const * address_, enet_uint16 port_)
	{
		ENetAddress address;
		ENetPeer * peer = 0;

		enet_address_set_host (& address, address_);
		address.port = port_;
		
		peer = enet_host_connect(_server, &address, All_Channels);

		if (!peer){ return false; }

		ENetEvent event;
		if(enet_host_service (_server, & event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			return true;
		}
		else
		{
			enet_peer_reset (peer);
			return false;
		}
	}

	void NetworkProcessManager::update(void)
	{
		ENetEvent event;
		static const enet_uint32 MAX_WAIT_MS = 100;

		{
			std::queue<Channels> * messages = _messages.get(); 

			while (!messages->empty())
			{
				for(std::set<ENetPeer *>::iterator i = _peers.begin(); i != _peers.end(); ++i)
				{
					ENetPacket * packet = enet_packet_create ("0", 1, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send (*i, messages->front(), packet);
					messages->pop();
				}
			}			

			delete messages;
		}

		while (enet_host_service (_server, &event, MAX_WAIT_MS) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				{
					_peers.insert(event.peer);
				}
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				{
					switch(event.channelID)
					{
					case New_Process:
						{
							NetworkProcess * networkProcess = createNetworkProcess(event.peer);
							_jobManager->addProcess(networkProcess);
						}
						break;
					case New_Job:
						{
							std::string job;
							for (int i = 1; i < event.packet->dataLength; ++i)
							{
								job += static_cast<char>(event.packet->data[i]);
							}
							
							_jobManager->queueJob(event.packet->data[0], job);
							enet_packet_destroy (event.packet);
						}
						break;
					case Process_Unavailable:
						{
							for (std::list<NetworkProcess *>::iterator i = _networkProcesses.begin();
								i != _networkProcesses.end(); ++i)
							{
								if( (*i)->isPeer(event.peer) )
								{
									destroyNetworkProcess(*i);
									_networkProcesses.erase(i);	
									break;
								}
							}							
						}
						break;
					}
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				{
					std::list<NetworkProcess *>::iterator i = _networkProcesses.begin(), j;
					while (i != _networkProcesses.end())
					{
						if((*i)->isPeer(event.peer))
						{	
							j = i; ++i;
							destroyNetworkProcess(*j);
							_networkProcesses.erase(j);	
						}
						else
						{
							++i;
						}
					}

					_peers.erase(event.peer);
				}
				break;
			}
		}
	}

	void NetworkProcessManager::queueJob(Job * job_)
	{
		_jobManager->queueJob(job_);
	}

	void NetworkProcessManager::signalJobFinished(Process * )
	{
		_messages.push(New_Process);
	}

	void NetworkProcessManager::signalJobTaken(Process * process_)
	{
		if(process_->id() == NetworkProcessId)
		{
			for (std::list<NetworkProcess *>::iterator i = _networkProcesses.begin();
				i != _networkProcesses.end(); ++i)
			{
				if( (*i) == process_ )
				{
					delete (*i); 
					_networkProcesses.erase(i);
					return;
				}
			}
		}
		else
		{
			_messages.push(Process_Unavailable);
		}
	}

	void NetworkProcessManager::sendJob(Job * job_, ENetPeer * peer_)
	{
		JobFactory * jobFactory = _jobManager->getJobFactory(job_->getJobFactoryId());
		if(!jobFactory){ return; }

		std::string job = jobFactory->id() + jobFactory->create(job_);
		ENetPacket * packet = enet_packet_create (job.c_str(), job.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		packet->data[job.size()] = 0;
		enet_peer_send (peer_, New_Job, packet);
	}

	NetworkProcess * NetworkProcessManager::createNetworkProcess(ENetPeer * peer_)
	{
		return new NetworkProcess(peer_, this);
	}

	void NetworkProcessManager::destroyNetworkProcess(NetworkProcess * networkProcess_)
	{
		_jobManager->remProcess(networkProcess_);
		delete networkProcess_;
	};
}
