#ifndef _AM_General_hpp_
#define _AM_General_hpp_

#include <map>
#include <set>
#include <string>
#include <boost/shared_ptr.hpp>

#include "AM_GlobalStringTable.hpp"

namespace AutoMatic
{
	class State;
	class Action;
	class Object;	
	class Listener;

	typedef StringValue StateId;
	typedef StringValue ActionId;
	typedef StringValue ListenerId;

	typedef boost::shared_ptr<State> StatePtr;
	typedef boost::shared_ptr<Action> ActionPtr;
	typedef boost::shared_ptr<Object> ObjectPtr;
	typedef boost::shared_ptr<Listener> ListenerPtr;

	typedef std::set<StateId> StateList;
	typedef std::set<Object *> ObjectList;
	typedef std::set<ObjectPtr> ObjectPtrList;
	typedef std::set<ListenerPtr> ListenerPtrList;

	typedef std::map<StateId, StatePtr> States;
	typedef std::map<ActionId, ActionPtr> Actions;
	typedef std::map<StateId, ListenerPtrList > StateListeners;
	typedef std::map<ActionId, ListenerPtrList > ActionListeners;	
}

#endif // _AM_General_hpp_
