#ifndef _AM_Object_hpp_
#define _AM_Object_hpp_

#include "AM_State.hpp"
#include "AM_General.hpp"

namespace AutoMatic
{
	/// An Object holds state and actions, as well as listeners to send messages to.
	/** 
	***
	*/
	class Object
	{
	public:
		
		/// Set a state of this Object.
		/** This will automatically send a state changed message to all listeners
			* on the state.
			* The setState function should be called to create all of the states that 
			* an object needs.
		*/
		void setState(StateId const & stateId_, StatePtr const & state_);

		/// Set the data of a state.
		/** \note This will crash if the state does not exist.
		*/
		template<class Data>
		void setData(StateId const & stateId_, Data const & data_)
		{
			DataState<Data>::GetData(_states[stateId_]) = data_;
			sendStateChangeMessage(stateId_);
		}

		/// Set an action of this Object.
		void setAction(ActionId const & actionId_, ActionPtr const & action_);

		/// Execute an action of this Object.
		void runAction(ActionId const & actionId_, States const & states_);

		/// The current state of the object.
		States & getStates(void) { return _states; }
		States const & getStates(void) const { return _states; }

		/// Get the data of the state.
		/** \note This will crash if the state does not exist.
		*/
		template<class Data>
		Data & getData(StateId const & stateId_)
		{
			return DataState<Data>::GetData(_states[stateId_]);
		}

		/// Has the Object got the State with \a stateId_?
		/** A helper function that calls State::HasState.
		*/
		bool hasState(StateId const & stateId_) const;

		/// Add a state listener to this object.
		void addStateListener(StateId const & stateId_, ListenerPtr const & listenerPtr_);

		/// Remove a state listener from this object.
		void remStateListener(StateId const & stateId_, ListenerPtr const & listenerPtr_);

		/// Add an action listener to this object.
		void addActionListener(ActionId const & actionId_, ListenerPtr const & listenerPtr_);

		/// Remove an action listener from this object.
		void remActionListener(ActionId const & actionId_, ListenerPtr const & listenerPtr_);

		/// Send a message to all listeners, listening for state changes on the state.
		void sendStateChangeMessage(StateId const & stateId_);

		/// Send a message to all listeners, listening for the action execution.
		void sendActionRunMessage(ActionId const & actionId_);

	private:

		States _states; /**< The states of this object. **/
		Actions _actions; /**< The actions of this object. **/

		StateListeners _stateListeners; /**< This state listeners of this object. **/
		ActionListeners _actionListeners; /**< The action listeners of this object. **/
	}; 

	ObjectPtr NewObjectPtr(void);

	void RunObjectTest(void);
}

#endif // _AM_Object_hpp_
