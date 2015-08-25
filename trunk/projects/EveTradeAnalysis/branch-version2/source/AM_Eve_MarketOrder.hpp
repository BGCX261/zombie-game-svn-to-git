#ifndef _AM_Eve_MarketOrder_hpp_
#define _AM_Eve_MarketOrder_hpp_

#include <string>

namespace AutoMatic
{
	namespace Eve
	{
		class MarketOrder
		{
		public:
			MarketOrder(void);

			double _price, _volRemaining;
			int _typeId, _range, _orderId, _volEntered, _minVolume, _duration, _stationId, _regionId, _systemId, _jumps;
			bool _isBid;
			std::string _issued;
		};
	}
}

#endif //_AM_Eve_MarketOrder_hpp_
