#ifndef _AM_Parsers_hpp_
#define _AM_Parsers_hpp_

#include <string>

namespace AutoMatic
{
	namespace Eve
	{
		class MarketAnalyser;
		class MarketOrder;
		class Jumps;
		
		bool LoadJumps(Jumps & jumps_, std::string const & jumpsFilename_, bool bailOnError_ );
		bool LoadSystems(Jumps & jumps_, std::string const & systemsFilename_, bool bailOnError_);
		bool LoadItemVolumes(MarketAnalyser & analyser_, std::string const & itemVolumesFile_);
		bool LoadOrders(MarketAnalyser & analyser_, std::string const & filename_);

		bool ParseMarketOrder(MarketOrder & order_, std::string const & input_);

		bool ParseSystem(Jumps & jumps_, std::string const & input_);

		bool ParseJumps(Jumps & jumps_, std::string const & input_);

		bool ConvertDBItemsToCSV(std::string const & filenameIn_, std::string const & filenameOut_);

		bool ConvertDBJumpsToCSV(std::string const & filenameIn_, std::string const & filenameOut_);

		bool ConvertDBSolarSystemToCSV(std::string const & filenameIn_, std::string const & filenameOut_);
	}
}

#endif //_AM_Parsers_hpp_