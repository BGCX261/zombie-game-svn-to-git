#ifndef _AM_Action_hpp_
#define _AM_Action_hpp_

#include "AM_General.hpp"

namespace AutoMatic
{
	class Object;

	/// An Action is run on the state of an Object.
	class Action
	{
	public:
		/// Run the action.  
		/** Derive from this, have the Action modify the states of the \a parent_.
			* After this method exits, the \a parent_ object will automatically 
			* send a message to all listeners on the action.
		*/
		virtual void run(Object & parent_, States const & arguments_) = 0;

		static ActionId const UniversalActionId; /**< Use to indicate all actions. **/

	private:

	};
}

#endif // _AM_Action_hpp_