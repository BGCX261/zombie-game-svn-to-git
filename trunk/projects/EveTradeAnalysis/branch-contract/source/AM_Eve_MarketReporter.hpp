#ifndef _AM_Eve_MarketReporter_hpp_
#define _AM_Eve_MarketReporter_hpp_

#include <string>

#include "AM_FileWatcher.hpp"
#include "AM_Eve_MarketAnalyser.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		class Jumps;
		class MarketReporter : public Reporter
		{
		public:
			MarketReporter(void);

			void operator()(FileWatcher::StringVec const & changedFiles_);
			
			void setPath(std::string const & path_){ _path = path_; }
			void setAnalyser(MarketAnalyser * analyser_){ _analyser = analyser_; }

			virtual void report();

		public:
			std::string _path, _file;
			MarketAnalyser * _analyser;
							
			std::list<ProfitIndex> _profits;
			GeneralOrderInfo _info;
		};
	}
}

#endif //_AM_Eve_MarketReporter_hpp_
