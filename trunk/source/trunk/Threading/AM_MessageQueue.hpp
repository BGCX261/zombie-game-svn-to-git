#ifndef _AM_MessageQueue_hpp_
#define _AM_MessageQueue_hpp_

#include <queue>
#include <boost/thread/condition.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace AutoMatic
{
	/// The MessageQueue class is a ThreadSafe class that holds messages sent to an object.
	template<class Message>
	class MessageQueue
	{
	public:
		typedef std::queue<Message> Queue;
		typedef std::pair<boost::recursive_mutex, Queue *> Messages;
		typedef boost::recursive_mutex::scoped_lock MessagesLock;

		MessageQueue(void) : _cancelWait(false)
		{
			_messages.second = new Queue;
		}

		~MessageQueue(void)
		{
			MessagesLock messagesLock(_messages.first);
			delete _messages.second; _messages.second = 0;
			cancelWait();
		}

		/// Push a message onto the message queue.
		/** \return False if the Queue no longer exists, true otherwise.
		*** \note This will cause any (or the next) waiting thread to continue.
		*/
		bool push(Message const & message_)
		{
			MessagesLock messagesLock(_messages.first);
			bool pushed = false;
			if(_messages.second)
			{
				pushed = true;
				_messages.second->push(message_);
			}

			cancelWait();

			return pushed;
		}

		/// Get the current Queue of Message's.
		/** \return Queue containing messages, or null.
		*** \note It is the caller's responsibility to delete the returned pointer.
		*/
		Queue * get(void)
		{
			MessagesLock messagesLock(_messages.first);
			Queue * q = _messages.second;
			_messages.second = new Queue;
			return q;
		}

		/// Wait until there is a Message in the Queue.
		/** \note Returns immediately in the Queue is null, there are no messages,
		*** or cancelWait() has been called.
		*/
		void waitForMessage(void)
		{
			MessagesLock messagesLock(_messages.first);
			if(!_messages.second || _messages.second->size() > 0){ return; }
			if(_cancelWait){ _cancelWait = false; return; }
			_waitForMessageCondition.wait(messagesLock);
		}

		/// Check if there is at least one message on the queue.
		bool hasMessage(void)
		{
			MessagesLock messagesLock(_messages.first);
			return (_messages.second && _messages.second->size() > 0);
		}
		
		/// Signal any threads waiting on a message to continue.
		/** If no threads are currently waiting, the next thread to wait will 
		*** immediately continue.
		*/
		void cancelWait(void)
		{
			MessagesLock messagesLock(_messages.first);
			_cancelWait = true;
			_waitForMessageCondition.notify_all();
		}

	private:
		bool _cancelWait; /**< Stop any threads waiting on a message. **/
		Messages _messages; /**< A queue of all waiting messages, if any. **/
		boost::condition _waitForMessageCondition; /**< Condition on which a thread waits for messages, triggered when a new message comes in of cancelWait is called. **/
	};
}

#endif //_AM_MessageQueue_hpp_