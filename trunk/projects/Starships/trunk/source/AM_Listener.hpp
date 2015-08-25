#ifndef _AM_Listener_hpp_
#define _AM_Listener_hpp_

#include "AM_General.hpp"

namespace AutoMatic
{
	/// A Listener is used to pass information about an object, to another object.
	/** 
	***
	*/
	class Listener
	{
	public:
		static ListenerPtr New(ListenerId const & listenerId_, ActionId const & actionId_);

		Listener(ListenerId const & listenerId_, ActionId const & actionId_);

		/// Receive Object state from \a sender_.
		void receive(Object const & sender_);

		/// Add a state to retrieve from a sender_.
		void addState(StateId const & stateId_);

		/// Remove a state to retrieve from a sender_.
		void remState(StateId const & stateId_);

		/// Add an object to send messages to.
		void addParent(Object * parent_);

		/// Remove an object to send messages to.
		void remParent(Object * parent_);

		static ListenerId const UniversalListenerId; /**< Use the universal id to find a listener id in States. **/

	private:
		ObjectList _parents; /**< The Object to send state information to. **/
		StateList _stateList; /**< The state information to send. **/
		ActionId _actionId; /**< The action to call on the object. **/
		ListenerId _listenerId; /**< The Id of the Listener, sent to the action in the arguments. **/

		bool _universalListener; /**< Will the listener report any state change? **/
	};
}

#endif // _AM_Listener_hpp_
