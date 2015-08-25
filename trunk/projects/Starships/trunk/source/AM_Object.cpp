#include "AM_Action.hpp"
#include "AM_Listener.hpp"

#include "AM_Object.hpp"

namespace AutoMatic
{
	ObjectPtr NewObjectPtr(void)
	{ 
		return ObjectPtr(new Object); 
	}

	void Object::setState(StateId const & stateId_, StatePtr const & state_)
	{
		_states[stateId_] = state_;

		sendStateChangeMessage(stateId_);
	}

	void Object::setAction(ActionId const & actionId_, ActionPtr const & action_)
	{
		_actions[actionId_] = action_;
	}

	void Object::runAction(ActionId const & actionId_, States const & states_)
	{
		Actions::iterator actions = _actions.find(actionId_);
		if(actions == _actions.end()){ return; }

		ActionPtr & action = (*actions).second;
		if(action)
		{
			action->run(*this, states_);
			sendActionRunMessage(actionId_);
		}
	}

	bool Object::hasState(StateId const & stateId_) const
	{
		return State::HasState(_states, stateId_);
	}

	void Object::addStateListener(StateId const & stateId_, ListenerPtr const & listenerPtr_)
	{
		_stateListeners[stateId_].insert(listenerPtr_);
	}

	void Object::remStateListener(StateId const & stateId_, ListenerPtr const & listenerPtr_)
	{
		_stateListeners[stateId_].erase(listenerPtr_);
	}

	void Object::addActionListener(ActionId const & actionId_, ListenerPtr const & listenerPtr_)
	{
		_actionListeners[actionId_].insert(listenerPtr_);
	}

	void Object::remActionListener(ActionId const & actionId_, ListenerPtr const & listenerPtr_)
	{
		_actionListeners[actionId_].erase(listenerPtr_);
	}

	void Object::sendStateChangeMessage(StateId const & stateId_)
	{
		StateListeners::iterator stateListeners = _stateListeners.find(stateId_);
		if(stateListeners == _stateListeners.end()){ return; }
		
		ListenerPtrList & listenerPtrList = (*stateListeners).second;
		for (ListenerPtrList::iterator i = listenerPtrList.begin(); i != listenerPtrList.end(); ++i)
		{
			(*i)->receive(*this);
		}

		if(stateId_ != State::UniversalStateId)
		{ // Send to all listeners waiting on any State change.
			sendStateChangeMessage(State::UniversalStateId);
		}
	}

	void Object::sendActionRunMessage(ActionId const & actionId_)
	{
		ActionListeners::iterator actionListeners = _actionListeners.find(actionId_);
		if(actionListeners == _actionListeners.end()){ return; }
		
		ListenerPtrList & listenerPtrList = (*actionListeners).second;
		for (ListenerPtrList::iterator i = listenerPtrList.begin(); i != listenerPtrList.end(); ++i)
		{
			(*i)->receive(*this);
		}

		if(actionId_ != Action::UniversalActionId)
		{
			sendActionRunMessage(Action::UniversalActionId);
		}
	}
}
