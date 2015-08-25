#ifndef _AM_ObjectFactory_hpp_
#define _AM_ObjectFactory_hpp_

#include "AM_General.hpp"

namespace AutoMatic
{
	/// Use ObjectFactories to create specialised Objects.
	/** The ObjectFactory for a specific object should provide methods for reading
	*** in data (and saving data?) in a variety of ways, and creating an object
		* from the data.
		*
		* \todo Possibly serialisation should be registered statically with DataState's.
	*/
	class ObjectFactory
	{
	public:
		virtual ObjectPtr build(void) = 0;
	};
}

#endif //_AM_ObjectFactory_hpp_
