#include "AM_Eve_MarketOrder.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		MarketOrder::MarketOrder(void)
			: _price(0.0), _volRemaining(0.0)
			, _typeId(-1), _range(-1), _orderId(-1), _volEntered(-1), _minVolume(-1)
			, _isBid(false), _issued("NULL")
			,	_duration(-1), _stationId(-1), _regionId(-1), _systemId(-1), _jumps(-1)
		{

		}
	}
}
