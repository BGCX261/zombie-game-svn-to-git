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

		}
	}
}
