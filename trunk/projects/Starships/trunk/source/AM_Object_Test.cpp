#include <cassert>
#include "AM_State.hpp"
#include "AM_Action.hpp"
#include "AM_Object.hpp"
#include "AM_Listener.hpp"

namespace AutoMatic
{
	namespace Strings
	{
		StringValue const IntState = GlobalStringTable::Add("IntState");
		StringValue const FloatState = GlobalStringTable::Add("FloatState");
		StringValue const StringState = GlobalStringTable::Add("StringState");

		StringValue const TestAction = GlobalStringTable::Add("TestAction");
		StringValue const TestObjectActionListener = GlobalStringTable::Add("TestObjectActionListener");
		StringValue const TestObjectStateListener = GlobalStringTable::Add("TestObjectStateListener");

		StringValue const TestActionListener = GlobalStringTable::Add("TestActionListener");
		StringValue const TestStateListener = GlobalStringTable::Add("TestStateListener");
		
	}

	/// 
	/** Action to call when the TestListener listening for the action is triggered.
	***
	*/
	class TestObjectActionListener : public Action
	{
	public:
		TestObjectActionListener(void)
		{	}

		virtual void run(Object & parent_, States const & arguments_)
		{
			ListenerId const & listenerId = DataState<ListenerId>::GetData(arguments_, Listener::UniversalListenerId);

			assert(Strings::TestActionListener == listenerId);

			assert(State::HasState(arguments_, Strings::IntState));
			assert(State::HasState(arguments_, Strings::FloatState));
			assert(State::HasState(arguments_, Strings::StringState));

			int intState = DataState<int>::GetData(arguments_, Strings::IntState);
			float floatState = DataState<float>::GetData(arguments_, Strings::FloatState);
			std::string stringState = DataState<std::string>::GetData(arguments_, Strings::StringState);

			assert(intState == 321);
			assert(floatState== 321.321f);
			assert(stringState == "stringTest");

			parent_.setState(Strings::IntState, NewDataState<int>(665));

			parent_.setData(Strings::IntState, 665);
		}


	};

	/// 
	/** Action to call when the TestListener listening for the State change is triggered.
	***
	*/
	class TestObjectStateListener : public Action
	{
	public:
		TestObjectStateListener(void)
			: _debugCountOfIntStateChange(0)
		{}

		virtual void run(Object & parent_, States const & arguments_)
		{
			ListenerId const & listenerId = DataState<ListenerId>::GetData(arguments_, Listener::UniversalListenerId);

			assert(Strings::TestStateListener == listenerId);

			_debugCountOfIntStateChange++;
		}

		int _debugCountOfIntStateChange;
	};

	class TestAction : public Action
	{
	public:
		virtual void run(Object & parent_, States const & arguments_)
		{
			//parent_.setState("IntState", StatePtr(new DataState<int>(321)));
			parent_.setState(Strings::IntState, NewDataState<int>(321));
			parent_.setState(Strings::FloatState, NewDataState<float>(321.321f));
			parent_.setState(Strings::StringState, NewDataState<std::string>("stringTest"));
		}
	};

	/// This test ensures the functionality of the Object class.
	/** 
	***
	*/
	void RunObjectTest(void)
	{
		Object testObject;

		testObject.setState(Strings::IntState, NewDataState<int>(123));
		testObject.setState(Strings::FloatState, NewDataState<float>(123.123f));
		testObject.setState(Strings::StringState, NewDataState<std::string>("testString"));

		assert(testObject.getStates().size() == 3);

		testObject.setAction(Strings::TestAction, ActionPtr( new TestAction ));
		testObject.setAction(Strings::TestObjectActionListener, ActionPtr(new TestObjectActionListener));

		TestObjectStateListener * testObjectStateListener = new TestObjectStateListener;

		testObject.setAction(Strings::TestObjectStateListener, ActionPtr(testObjectStateListener));

		ListenerPtr testActionListener = Listener::New(Strings::TestActionListener, Strings::TestObjectActionListener);
		testActionListener->addParent(&testObject);

		// Set the states to report on.
		testActionListener->addState(Strings::IntState);
		testActionListener->addState(Strings::FloatState);
		testActionListener->addState(Strings::StringState);

		// Set the action to react to.
		testObject.addActionListener(Strings::TestAction, testActionListener);
		
		ListenerPtr testStateListener = Listener::New(Strings::TestStateListener, Strings::TestObjectStateListener);
		testStateListener->addParent(&testObject);

		// Set the state to report on.
		testStateListener->addState(Strings::IntState);

		// Set the state to react to.
		testObject.addStateListener(Strings::IntState, testStateListener);

		testObject.runAction(Strings::TestAction, States());

		assert(testObject.hasState(Strings::IntState));
		int intState = DataState<int>::GetData(testObject.getStates(), Strings::IntState);
		assert(intState == 665);

		testObject.setData<int>(Strings::IntState, 777);
		assert(testObject.getData<int>(Strings::IntState) == 777);

		assert(testObjectStateListener->_debugCountOfIntStateChange == 4);
	}
}
