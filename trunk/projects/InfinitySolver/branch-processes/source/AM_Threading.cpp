#include "AM_Threading.hpp"

#include "AM_Memory.h"

namespace AutoMatic
{
	void Thread::ThreadHolder::operator()(void)
	{
		_thread->loop();
		//_thread = 0; // This could be dangerous, if release is called before the 
		// thread has reached this point.  Also, it is unnecessary, because if attach 
		// is called without release, the _holder is replaced anyway.
	}

	void Thread::setTerminate(bool terminate)
	{
		boost::recursive_mutex::scoped_lock terminateLock( _terminate.first );
		_terminate.second = terminate; 
	}

	void Thread::setIsTerminated(bool isTerminated)
	{
		boost::recursive_mutex::scoped_lock isTerminatedLock( _isTerminated.first );
		_isTerminated.second = isTerminated;
	}

	Thread::Thread(void)
	{
		setIsTerminated(true);
	}

	Thread::~Thread(void)
	{
		terminate();
		release();
	}

	void Thread::attach( void )
	{
		// isTerminated is true before _terminateCondition signals.
		// Therefore, this lock (also set in terminate) is needed to prevent a 
		// THIRD (or fourth, etc.) party thread from calling attach before the
		// thread is ready.
		boost::recursive_mutex::scoped_lock releaseLock(_release);
		if( isTerminated() )
		{ // isTerminated is true by default.
			// By calling setIsTerminated(false) here, there is the possibility of
				// another thread calling terminate() before this Thread has a chance
				// to enter its loop.  Which is fine, because the called of terminate()
				// waits until this Thread has exited the loop and signalled the 
				// _terminateCondition
			setIsTerminated(false);
			boost::shared_ptr<ThreadHolder> holder(new ThreadHolder( this ));
			boost::shared_ptr<boost::thread> boostThread( new boost::thread( *holder ) );
			// thread->loop() COULD run in here... so thread must NOT rely on having 
			// _boostThread set... technically, it shouldn't even know boost::thread's are being used.
			_holder = holder;
			_holder->setBoostThread(boostThread);

			// Have this calling thread wait until the loop is entered... shouldn't take long,
				// and provides peace of mind about the method call's to this Thread.
			waitUntilThreadEntersLoop(); // TODO: Should this be an option for the user of Thread?
		}
	}

	void Thread::release(void)
	{
		// isTerminated is true before _terminateCondition signals.
			// Therefore, this lock (also set in terminate) is needed to prevent a 
			// THIRD (or fourth, etc.) party thread from calling release before the
			// thread is ready.
		boost::recursive_mutex::scoped_lock releaseLock(_release);
		if(isTerminated())
		{ // TODO: What if this is called before the ThreadHolder exits operator()??
				// SHOULD be safe, because the operator() has no code after exiting the loop.
			_holder.reset();
		}
	}

	void Thread::setIsPaused(bool isPaused)
	{
		boost::recursive_mutex::scoped_lock isPausedLock( _isPaused.first );
		_isPaused.second = isPaused;
		if(isPaused)
		{ // waitForUnpause
			// Tell all waiting for this thread to pause, that it is paused.
			_waitForPauseCondition.notify_all(); 
			_pauseCondition.wait( isPausedLock );
		}
	}

	void Thread::waitForUnpause(void)
	{
		if( isPauseRequested() )
		{
			setIsPaused(true); // Note, the thread will wait for unpause(),
			setIsPaused(false); // So this WON'T happen immediately!
		}
	}

	void Thread::signalHasEnteredLoop(bool hasEntered)
	{
		boost::recursive_mutex::scoped_lock isEnteringLoopLock( _isEnteringLoop.first );
		_isEnteringLoop.second = hasEntered;
		if(hasEntered)
		{
			_isEnteringLoopCondition.notify_all();
		}
	}

	void Thread::waitUntilThreadEntersLoop(void)
	{
		boost::recursive_mutex::scoped_lock isEnteringLoopLock( _isEnteringLoop.first );
		if(!_isEnteringLoop.second)
		{
			_isEnteringLoopCondition.wait(isEnteringLoopLock);
		}
	}

	void Thread::loop(void)
	{
		signalHasEnteredLoop(true);
		while(true)
		{
			boost::thread::yield(); // TODO: Needed?
			//**********************************************************************//
			waitForUnpause();
			//**********************************************************************//
			if(isTerminateRequested())
			{ 
				setIsTerminated(true);
				break; 
			}
			//**********************************************************************//
			update();
			//**********************************************************************//
		}

		// This will make sure that the caller of terminate is actually waiting on the condition!
		boost::recursive_mutex::scoped_lock terminateLock( _terminate.first );
		_terminateCondition.notify_all();
		signalHasEnteredLoop(false);
	}

	void Thread::requestPause(void)
	{
		boost::recursive_mutex::scoped_lock isPausedRequestedLock( _isPausedRequested.first );
		_isPausedRequested.second = true;
	}

	void Thread::unpause(void)
	{
		boost::recursive_mutex::scoped_lock isPausedRequestedLock( _isPausedRequested.first );
		_isPausedRequested.second = false;
		_pauseCondition.notify_all();
	}

	bool Thread::isPauseRequested(void)
	{
		boost::recursive_mutex::scoped_lock isPausedRequestedLock( _isPausedRequested.first );
		return _isPausedRequested.second;
	}

	bool Thread::isPaused(void)
	{
		boost::recursive_mutex::scoped_lock isPausedLock( _isPaused.first );
		return _isPaused.second;
	}

	void Thread::waitUntilPaused(void)
	{
		if(!isPauseRequested()){ return; }
		boost::recursive_mutex::scoped_lock isPausedLock( _isPaused.first );
		if(_isPaused.second){ return; }
		_waitForPauseCondition.wait(isPausedLock);
	}

	void Thread::waitUntilTerminated(void)
	{
		boost::recursive_mutex::scoped_lock terminateLock( _terminate.first );
		if(!_terminate.second)
		{
			// Tell the thread to terminate and wait until it has done so.
			_terminateCondition.wait( terminateLock );
		}
	}

	bool Thread::isTerminated(void)
	{
		boost::recursive_mutex::scoped_lock isTerminatedLock( _isTerminated.first );
		return _isTerminated.second;
	}

	bool Thread::isAttached(void)
	{
		return !isTerminated();
	}

	bool Thread::isTerminateRequested(void)
	{
		boost::recursive_mutex::scoped_lock terminateLock( _terminate.first );
		return _terminate.second;
	}

	void Thread::terminate(void)
	{ 
		// This lock will prevent any third thread from releasing the thread 
			// before the thread has called _terminateCondition.notify_all().
			// (It is locked in release as well).
		boost::recursive_mutex::scoped_lock releaseLock(_release);
		if( !isTerminated() )
		{ // If the thread has not terminated
			unpause(); // No good to keep the Thread paused...
			boost::recursive_mutex::scoped_lock terminateLock( _terminate.first );
			_terminate.second = true; 
			// Tell the thread to terminate and wait until it has done so.
			_terminateCondition.wait( terminateLock );
		}
	}
}

