#ifndef _AutoMatic_Thread_h_
#define _AutoMatic_Thread_h_

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

#include <boost/shared_ptr.hpp>

#include <list>

namespace AutoMatic
{
	/// Inherit from this class to create new thread types.
	/** This class ... is a bloody work of art.
	*/
	class Thread
	{
	public:

		/// Sets _isTerminated to true, because technically it is (the thread is not running yet).
		Thread(void);

		virtual ~Thread(void);

		/// This method is called once every thread loop.
		/** Overload this method in derived classes.
		*/
		virtual void update(void){}

		/// Attach this class to a boost::thread.
		/** A Thread may only be attached to one boost::thread at a time.
			* Successive calls do nothing, until the current thread has terminated.
			* I.e., terminate() has been called and isTerminated() is true.
			*
			* This will create a new boost::thread that will indirectly call loop().
		*/
		void attach(void);

		/// Releases the boost::thread and _holder memory.
		/** The method does not necessarily have to be called.
			* Call terminate before calling this method.
			* Calling before _isTerminated does nothing.
			* Calling before attach() does nothing.
			*
			* \todo What if this is called before the ThreadHolder exits operator()?
			* No member variables are touched (or methods called), so it Should be safe.
			* Maybe I should just remove this method?
		*/
		void release(void);

		/// Pause this thread until unpause is called.
		virtual void requestPause(void);
		/// Unpause this thread.
		void unpause(void);
		/// Has pause() been called?
		bool isPauseRequested(void);
		/// Is the thread paused?
		bool isPaused(void);
		/// Wait a thread until this Thread is paused.
		/** Will not wait if pause is not requested.
		*/
		void waitUntilPaused(void);

		/// When terminate is called, the caller blocks until the Thread has exited.
		/** This will unpause this Thread before terminating.
			* Because it blocks, don't call terminate() from the Thread being terminated.
			* To do that, call setTerminate(true) instead.
			*
			* Calling this method does not delete Thread (or derived) data.  
			* It doesn't even delete boost::thread or ThreadHolder data.  It simply 
			* exits the Thread loop().  As such, to go from single to multi and back
			* is entirely possible within on app run!
		*/
		void terminate(void);

		/// Returns true if the thread has exited (or is about to exit) the loop.
		/** isTerminated is true by default.  
			*	It is set to false in the attach method before creating the new 
			* boost::thread.
		*/
		bool isTerminated(void);

		/// Returns true if the thread is not terminated.
		bool isAttached(void);

		/// Returns true if another thread has called terminate().
		bool isTerminateRequested(void);

	protected:
		//************************************************************************//
		/// The ThreadHolder has the method called by the boost::thread.
		/**	The only reason ThreadHolder exists is to prevent large-scale copying
		* of the Thread class (multiple copies are made of the object passed to the
		*	boost::thread constructor). This is also why it is private.
		*/
		class ThreadHolder
		{
		private:
			Thread * _thread;
			boost::shared_ptr<boost::thread> _boostThread; /**< Stores the main thread. */

		public:
			ThreadHolder(Thread * thread)	: _thread(thread) {}
			ThreadHolder(ThreadHolder const & copy)	: _thread(copy._thread) {}

			~ThreadHolder(void){}

			ThreadHolder const & operator=(ThreadHolder const & copy)
			{
				_thread = copy._thread;
				return *this;
			}

			/// This method is called by the boost::thread automatically.
			/** This method then goes to call the Thread::loop() method.
			*/
			void operator()(void);

			/// Store the boost::thread pointer.
			void setBoostThread(boost::shared_ptr<boost::thread> const & thread)
			{
				_boostThread = thread;
			}
		};
		//************************************************************************//
		// Thread termination and pause system.  
		// Means of terminating/pausing the thread externally.
		boost::condition _terminateCondition, _pauseCondition, _waitForPauseCondition, _isEnteringLoopCondition;
		std::pair< boost::recursive_mutex , bool> _terminate, _isTerminated, _isPausedRequested, _isPaused,
			_isEnteringLoop;
		// The release mutex prevents a third thread from calling release() before this 
		// Thread has called _terminateCondition.notify_all().
		boost::recursive_mutex _release; /**< Locked in terminate(), release() and attach() */

		boost::shared_ptr< ThreadHolder > _holder; /**< Holds a pointer to this */

		/// Set the _terminate value to terminate.
		void setTerminate(bool terminate);

		/// Set the _isTerminated value to isTerminated.
		void setIsTerminated(bool isTerminated);

		/// Set the _isPaused value to isPaused, and waits until unpause if \a isPaused is true.
		void setIsPaused(bool isPaused);

		/// If another thread has called pause on this one, wait for unpause to be called.
		void waitForUnpause(void);

		/// This is called by the ThreadHolder created in attach.
		/** This method performs the basic pause and termination checks,
		*	and updates the Thread.
		*/
		void loop(void);

		/// This method is called when the new thread has entered loop().
		/** If \a hasEntered is true, this method signals all waiting on 
		* _isEnteringLoopCondition (i.e., those threads that called 
		* waituntilThreadEnteresLoop()).
		*/
		void signalHasEnteredLoop(bool hasEntered);
		void waitUntilThreadEntersLoop(void);

		/// Not implemented (on purpose, for now).
		Thread const & operator=(Thread const & copy);

		/// Not implemented (on purpose, for now).
		Thread(Thread const & copy);

	};
//****************************************************************************//
}

#endif _AutoMatic_Thread_h_


