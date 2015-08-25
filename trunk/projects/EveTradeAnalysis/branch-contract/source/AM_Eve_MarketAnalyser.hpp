#ifndef _AM_Eve_MarketAnalyser_hpp_
#define _AM_Eve_MarketAnalyser_hpp_

#include <map>
#include <list>
#include <string>
#include <functional>

#include "AM_Eve_MarketOrder.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		class Jumps;

		class ProfitIndex
		{
		public:
			ProfitIndex(void);

			std::string _file; /**< The file the profit is from. **/

			double _index; /**< The relative profit index of the trade route. **/

			double _profit; /**< The expected total profit after all trips. **/

			double _profitPerUnit; /**< The profit made for every unit sold. **/

			double _unitCost; /**< The cost per unit bought. **/

			double _unitIncome; /**< The income per unit sold. **/

			double _unitVolume; /**< Volume per unit in metres cubed. **/

			double _unitsShippedPerTrip; /**< The number of units to be carried per trip. **/

			double _tripVolume; /**< The volume shipped per trip. **/

			int _jumpsToStart; /**< The number of jumps required to start the trade route. **/

			int _jumpsPerTrip; /**< The number of jumps per trip. **/

			int _jumpsTotal; /**< The total number of jumps, over all trips. **/

			int _trips; /**< The number of trips required to transport all units. **/

			double _lowestSecurity; /**< The lowest security sector in the trip. **/

			bool _isEstimate; /**< The distance required to travel is only an estimate.  **/
		};

		struct SortSellOrders	: public std::binary_function<MarketOrder, MarketOrder, bool>
		{	
			bool operator()(MarketOrder const & lhs_, MarketOrder const & rhs_) const;
		};

		struct SortBuyOrders	: public std::binary_function<MarketOrder, MarketOrder, bool>
		{	
			bool operator()(MarketOrder const & lhs_, MarketOrder const & rhs_) const;
		};

		struct SortProfits	: public std::binary_function<MarketOrder, MarketOrder, bool>
		{	
			bool operator()(ProfitIndex const & lhs_, ProfitIndex const & rhs_) const;
		};

		struct GeneralOrderInfo
		{
			double _largestProfit;
			MarketOrder _highestBuyOrder;
			MarketOrder _lowestSellOrder;
		};

		class MarketAnalyser
		{
		public:
			static const int DEFAULT_MAX_JUMPS_TO_BUY_ORDER, DEFAULT_MAX_JUMPS_TO_SELL_ORDER;

			MarketAnalyser(void);

			void examineOrders(std::list<ProfitIndex> & profits_, GeneralOrderInfo & orderInfo_);

			ProfitIndex examineOrder(MarketOrder const & sellOrder_, MarketOrder const & buyOrder_);

			void setJumps(Jumps * jumps_){ _jumps = jumps_; }
			void setFunds(double funds_){ _funds = funds_; }
			void setVolume(double volume_){ _shipVolume = volume_; }
			void setUseROIAsIndex(bool useRoIAsIndex_){ _useRoIAsIndex = useRoIAsIndex_; }
			double funds(void) const { return _funds; }

			void setMaxJumpsToStart(int maxJumpsToStart_)
			{
				_maxJumpsToSellOrder = maxJumpsToStart_; 
			}

			void setMaxJumpsPerTrip(int maxJumpsPerTrip_)
			{
				_maxJumpsToBuyOrder = maxJumpsPerTrip_;
			}

			void setDebugOutput(bool enabled_){ _debugOutput = enabled_; }
			
			void ignoreJumpsToStart(bool ignore_){ _ignoreJumpsToStart = ignore_; }
			void ignoreJumpsPerTrip(bool ignore_){ _ignoreJumpsPerTrip = ignore_; }
			void ignoreSellOrderVolumeAbove(int volume_){ _ignoreSellOrderVolume = volume_; }

		private:
			friend bool LoadOrders(MarketAnalyser & analyser_, std::string const & filename_);
			friend bool LoadItemVolumes(MarketAnalyser & analyser_, std::string const & itemVolumesFile_);

			std::list<MarketOrder> _buyOrders, _sellOrders;
			std::map<int, double> _itemVolumes;
			double _largestProfit, _shipVolume, _funds;
			int _maxJumpsToSellOrder, _maxJumpsToBuyOrder;
			Jumps * _jumps;
			
			bool _debugOutput, _ignoreJumpsToStart, _ignoreJumpsPerTrip, _useRoIAsIndex;
			int _ignoreSellOrderVolume;
		};
	}
}

#endif //_AM_Eve_MarketAnalyser_hpp_
