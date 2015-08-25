#include <iostream>

#include "AM_Eve_MarketAnalyser.hpp"

#include "AM_Eve_MarketReporter.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		MarketReporter::MarketReporter(void)
			: _path("."), _analyser(0)
		{}

		void MarketReporter::operator()(FileWatcher::StringVec const & changedFiles_)
		{
			if(!_analyser){ return; }

			_profits.clear();
			
			for (FileWatcher::StringVec::const_iterator cur = changedFiles_.begin(); 
				cur != changedFiles_.end(); ++cur)
			{
				_file = *cur;
				std::string filename =  _path + "/" + _file;

				LoadOrders(*_analyser,filename);
				_analyser->examineOrders(_profits, _info);

				if(_profits.size() > 0)
				{
					report();
				}
			}
		}

		void MarketReporter::report(void)
		{
			/*
			std::cout.precision(2);
			for (std::list<ProfitIndex>::iterator i = _profits.begin(); i != _profits.end(); ++i)
			{
				if( (*i)._index < 0.0 ){ std::cout << "No profits to be had." << std::endl; }
				else 
				{
					std::cout << "Largest Profit: " << std::fixed << _largestProfit << std::endl;
					std::cout << std::endl << (int)(*i)._index 
						<< ": B: " << std::fixed << (*i)._units << " @ " << std::fixed << (*i)._sell._price << " V: " << std::fixed << (*i)._volume*(*i)._units
						<< ", S: " << std::fixed << (*i)._buy._price
						<< " P: " << std::fixed << (*i)._profit
						<< "\nJ: " << (*i)._jumpsToStart + (*i)._jumpsPerTrip << " (" << (*i)._jumpsToStart << ", " << (*i)._jumpsPerTrip << ")" 
						<< ((*i)._isEstimate ? "Estimate" : "")
						<< std::endl;

					if((*i)._sell._price * (*i)._units >= _analyser->funds() * 0.9)
					{
						std::cout << "This will chew up >= 90% of funds!" << std::endl;
					}
				}
				break;
			}
			*/
		}
	}
}
