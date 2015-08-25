#ifndef _AM_SolarSystem_hpp_
#define _AM_SolarSystem_hpp_

#include "AM_GameGeneral.hpp"

namespace AutoMatic
{
	/// A SolarSystem holds resources to be traded.
	/** SolarSystems are connected to each other.
	*** 
	*/
	class SolarSystemFactory : public ObjectFactory
	{
	public:
		SolarSystemFactory(void);

		virtual ObjectPtr build(void);
	};
}

#endif //_AM_SolarSystem_hpp_