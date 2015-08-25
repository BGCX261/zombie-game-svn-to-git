#ifndef _AM_State_hpp_
#define _AM_State_hpp_

#include "AM_General.hpp"

namespace AutoMatic
{
	/// A State holds data.
	/** Derive or implement template types (from DataState).
	***
	*/
	class State
	{
	public:
		/// Create a clone of this state (using new).
		virtual State * clone(void) const = 0;

		static bool HasState(States const & states_, StateId const & stateId_)
		{
			States::const_iterator stateIter = states_.find(stateId_);
			return (stateIter != states_.end());
		}

		static StateId const UniversalStateId; /**< Use the universal id to indicate a desire for all states. **/

	private:

	};

	/// \todo Possibly serialisation should be registered statically with DataState's.
	template<class Data>
	class DataState : public State
	{
	public:
		DataState(Data const & data_)
			: _data(data_)
		{}

		/// Retrieve the data of this DataState.
		Data & getData(void) { return _data; }

		virtual State * clone(void) const
		{
			return new DataState<Data>(_data);
		}

		/// Checks if the State of \a stateId_ is set in \a states_.
		/** Does not check data type.
		***
		*/
		static bool HasData(States const & states_, StateId const & stateId_)
		{
			States::const_iterator stateIter = states_.find(stateId_);
			if(states_.end() == stateIter){ return false; }
			if(!(*stateIter).second.get()){ return false; }
			return true;
		}

		/// A helper function to get data from a StatePtr
		/** \note It does not check if the StatePtr is of the correct type.  
			* Use at your own risk.
		*/
		static Data & GetData(StatePtr const & statePtr_)
		{
			return static_cast< DataState<Data> *> (statePtr_.get())->getData();
		}

		/// Get data from \a states_ with \a stateId_.
		/** \note It does not check if the StateId exists in the States.
		*** Check with HasData before using if unsure.
		*/
		static Data & GetData(States const & states_, StateId const & stateId_)
		{
			States::const_iterator stateIter = states_.find(stateId_);
			return GetData((*stateIter).second);
		}

	private:
		Data _data;
	};

	template<class Data>
	class NewDataState
	{
	public:
		NewDataState(Data const & data_)
			: _isStatePtrSet(false), _dataState(0)
		{
			_dataState = new DataState<Data>(data_);
		}

		~NewDataState(void)
		{
			if(!_isStatePtrSet){ delete _dataState; }
		}

		operator StatePtr()
		{
			_isStatePtrSet = true;
			return StatePtr(_dataState);
		}

	private:
		bool _isStatePtrSet;
		DataState<Data> * _dataState;
	};
}

#endif // _AM_State_hpp_
