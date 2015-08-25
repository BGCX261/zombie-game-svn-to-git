#include "AM_State.hpp"
#include "AM_Object.hpp"
#include "AM_Listener.hpp"

namespace AutoMatic
{
	ListenerId const Listener::UniversalListenerId = GlobalStringTable::Add("UniversalListenerId");

	ListenerPtr Listener::New(ListenerId const & listenerId_, ActionId const & actionId_)
	{ 
		return ListenerPtr(new Listener(listenerId_, actionId_)); 
	}

	Listener::Listener(ListenerId const & listenerId_, ActionId const & actionId_)
		: _listenerId(listenerId_), _actionId(actionId_)
	{

	}

	void Listener::receive(Object const & sender_)
	{
		States states;

		for (States::const_iterator i = sender_.getStates().begin(); i != sender_.getStates().end(); ++i)
		{ // For each of the object's states, 
			if(_universalListener || _stateList.find((*i).first) != _stateList.end())
			{ // if the listener is listening for changes in the state,
				states[(*i).first] = StatePtr((*i).second->clone()); // Add it to the list.
			}
		}

		states[UniversalListenerId] = NewDataState<ListenerId>(_listenerId);

		for (ObjectList::iterator i = _parents.begin(); i != _parents.end(); ++i)
		{ // Send the list to all of the parents.
			(*i)->runAction(_actionId, states);
		}
	}

	void Listener::addState(StateId const & stateId_)
	{
		if(stateId_ == State::UniversalStateId){ _universalListener = true; }
		else { _stateList.insert(stateId_); }
	}

	void Listener::remState(StateId const & stateId_)
	{
		if(stateId_ == State::UniversalStateId){ _universalListener = false; }
		else { _stateList.erase(stateId_); }
	}

	void Listener::addParent(Object * parent_)
	{
		_parents.insert(parent_);
	}

	void Listener::remParent(Object * parent_)
	{
		_parents.erase(parent_);
	}

}
