#include <cassert>

#include "AM_Eve_Jumps.hpp"
#include "AM_Eve_Parsers.hpp"
#include "AM_Eve_MarketAnalyser.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		ProfitIndex::ProfitIndex(void) 
			: _index(-1.0), _profit(0.0), _profitPerUnit(0.0), _unitCost(0.0), _unitIncome(0.0)
			,	_unitVolume(0.0), _unitsShippedPerTrip(0.0), _tripVolume(0.0), _jumpsToStart(0)
			, _jumpsPerTrip(0), _jumpsTotal(0), _trips(0), _isEstimate(false), _lowestSecurity(0)

		{

		}

		bool SortSellOrders::operator()(MarketOrder const & lhs_, MarketOrder const & rhs_) const
		{	
			return (lhs_._price < rhs_._price);
		}

		bool SortBuyOrders::operator()(MarketOrder const & lhs_, MarketOrder const & rhs_) const
		{	
			return (lhs_._price > rhs_._price);
		}

		bool SortProfits::operator()(ProfitIndex const & lhs_, ProfitIndex const & rhs_) const
		{	
			return (lhs_._index > rhs_._index);
		}

		const int MarketAnalyser::DEFAULT_MAX_JUMPS_TO_BUY_ORDER = 100;
		const int MarketAnalyser::DEFAULT_MAX_JUMPS_TO_SELL_ORDER = 100;

		MarketAnalyser::MarketAnalyser(void)
			: _jumps(0), _shipVolume(0.0), _funds(0.0)
			, _maxJumpsToSellOrder(DEFAULT_MAX_JUMPS_TO_SELL_ORDER)
			, _maxJumpsToBuyOrder(DEFAULT_MAX_JUMPS_TO_BUY_ORDER)
			, _debugOutput(false), _ignoreJumpsToStart(false), _ignoreSellOrderVolume(-1)
			, _useRoIAsIndex(false), _ignoreJumpsPerTrip(false)
		{
		}

		void MarketAnalyser::examineOrders(std::list<ProfitIndex> & profits_, GeneralOrderInfo & orderInfo_)
		{
			_largestProfit = 0.0;

			bool hasHighestBuyOrder = false;

			for (std::list<MarketOrder>::const_iterator i = _sellOrders.begin(); i != _sellOrders.end(); ++i)
			{
				if((*i)._price < orderInfo_._lowestSellOrder._price || i == _sellOrders.begin())
				{ 
					orderInfo_._lowestSellOrder = *i; 
				}

				for (std::list<MarketOrder>::const_iterator j = _buyOrders.begin(); j != _buyOrders.end(); ++j)
				{
					if(!hasHighestBuyOrder && ((*j)._price > orderInfo_._highestBuyOrder._price || j == _buyOrders.begin()))
					{ 
						orderInfo_._highestBuyOrder = *j;
					}

					profits_.push_back(examineOrder(*i, *j));
				}

				hasHighestBuyOrder = true;
			}

			profits_.sort(SortProfits());
			orderInfo_._largestProfit = _largestProfit;
		}

		ProfitIndex MarketAnalyser::examineOrder(MarketOrder const & sellOrder_, MarketOrder const & buyOrder_)
		{
			ProfitIndex profitIndex;

			profitIndex._unitCost = sellOrder_._price;
			profitIndex._unitIncome =  buyOrder_._price;

			profitIndex._profitPerUnit = profitIndex._unitIncome - profitIndex._unitCost;

			if( profitIndex._profitPerUnit <= 0.0 || 
					buyOrder_._jumps > _maxJumpsToBuyOrder || 
					sellOrder_._jumps > _maxJumpsToSellOrder)
			{ 
				profitIndex._index = -1.0;
				return profitIndex;
			}

			profitIndex._unitVolume = _itemVolumes[buyOrder_._typeId];

			double totalUnitsAvailable = ( (buyOrder_._volRemaining < sellOrder_._volRemaining)
				|| (_ignoreSellOrderVolume > -1 && _ignoreSellOrderVolume <= sellOrder_._volRemaining) ? 
				buyOrder_._volRemaining : sellOrder_._volRemaining);

			profitIndex._trips = 1;
			if(_shipVolume > 0.0)
			{
				profitIndex._trips = ceil((totalUnitsAvailable * profitIndex._unitVolume) / _shipVolume);
			}			

			assert(profitIndex._trips >= 1.0);

			profitIndex._unitsShippedPerTrip = totalUnitsAvailable;

			if(_shipVolume > 0.0 && profitIndex._unitVolume * profitIndex._unitsShippedPerTrip > _shipVolume)
			{ // Purchasable volume exceeds ship capacity
				profitIndex._unitsShippedPerTrip = _shipVolume / profitIndex._unitVolume;
			}

			if(_funds > 0.0 && sellOrder_._price*profitIndex._unitsShippedPerTrip > _funds)
			{ // Exceeds available funds!
				profitIndex._unitsShippedPerTrip = _funds / sellOrder_._price;
			}

			if(profitIndex._unitsShippedPerTrip < buyOrder_._minVolume)
			{ 
				profitIndex._index = -1.0;
				return profitIndex;
			}

			std::pair<int, double> jumpFromSellToBuy(-1, 0.0);
			if(_jumps)
			{
				jumpFromSellToBuy = _jumps->getJumps(
					Jumps::StationName(sellOrder_._regionId, sellOrder_._systemId), 
					Jumps::StationName(buyOrder_._regionId, buyOrder_._systemId));
				profitIndex._lowestSecurity = jumpFromSellToBuy.second;
			}

			if(jumpFromSellToBuy.first < 0)
			{ 
				profitIndex._isEstimate = true;
				jumpFromSellToBuy.first = buyOrder_._jumps; 
			}

			profitIndex._tripVolume = profitIndex._unitVolume * profitIndex._unitsShippedPerTrip;

			profitIndex._jumpsToStart = sellOrder_._jumps;
			profitIndex._jumpsPerTrip = jumpFromSellToBuy.first;

			profitIndex._jumpsTotal = (_ignoreJumpsToStart ? 0 : profitIndex._jumpsToStart) + (_ignoreJumpsPerTrip ? 1.0 : profitIndex._jumpsPerTrip)*profitIndex._trips;

			profitIndex._profit = profitIndex._profitPerUnit * profitIndex._unitsShippedPerTrip * profitIndex._trips;
			
			if(profitIndex._profit > _largestProfit){	_largestProfit = profitIndex._profit;	}

			double jumpDiv = profitIndex._trips + 2.0*( ((_ignoreJumpsPerTrip ? 1.0 : profitIndex._jumpsPerTrip)*profitIndex._trips) + (_ignoreJumpsToStart ? 0 : profitIndex._jumpsToStart) );

			double RoI = (profitIndex._profitPerUnit * profitIndex._unitsShippedPerTrip) / (profitIndex._unitsShippedPerTrip * profitIndex._unitCost);

			if(jumpDiv <= 0.0){ jumpDiv = 1.0; }

			profitIndex._index = (_useRoIAsIndex ? RoI / jumpDiv : profitIndex._profit / jumpDiv);

			return profitIndex;
		}
	}
}
