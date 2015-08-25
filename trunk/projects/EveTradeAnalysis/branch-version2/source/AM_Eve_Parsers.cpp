#include <fstream>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>

#include "AM_Eve_Jumps.hpp"
#include "AM_Eve_MarketOrder.hpp"
#include "AM_Eve_MarketAnalyser.hpp"

#include "AM_Eve_Parsers.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		bool LoadSystems(Jumps & jumps_, std::string const & systemsFilename_, bool bailOnError_ )
		{
			std::ifstream systemsFile(systemsFilename_.c_str());
			if(!systemsFile.is_open()){ return false; }

			jumps_._systemSecurity.clear();

			int line = 0;
			std::string buf;

			while (!systemsFile.eof())
			{
				try
				{
					systemsFile >> buf; line++;
					ParseSystem(jumps_, buf);
				}
				catch(boost::bad_lexical_cast &)
				{
					std::cout << "*** Corrupt Jumps File, line " << line << " ***" << std::endl;
					if(bailOnError_){ return false; }
				}	
			}

			return true;
		}

		bool LoadJumps(Jumps & jumps_, std::string const & jumpsFilename_, bool bailOnError_ )
		{
			delete jumps_._graph;
			jumps_._graph = new Jumps::Graph;
			jumps_._stations.clear();

			std::ifstream jumpsFile(jumpsFilename_.c_str());
			if(!jumpsFile.is_open()){ return false; }

			int line = 0;
			std::string buf;

			line = 0;
			// Read each line in the file
			while (!jumpsFile.eof())
			{
				try
				{	
					jumpsFile >> buf;	line++;
					ParseJumps(jumps_, buf);
				}
				catch(boost::bad_lexical_cast &)
				{
					if(bailOnError_){ return false; }
				}		
			}

			return true;
		}

		bool LoadOrders(MarketAnalyser & analyser_, std::string const & filename_)
		{
			analyser_._buyOrders.clear();
			analyser_._sellOrders.clear();

			std::ifstream file(filename_.c_str());
			assert(file.is_open());
			if(!file.is_open()){ return false; }
			std::string buf;
			file >> buf;

			int line = 1;
			while (!file.eof())
			{
				file >> buf;
				line++;
				try
				{
					MarketOrder order;
					ParseMarketOrder(order, buf);

					if(order._isBid){ analyser_._buyOrders.push_back(order); }
					else { analyser_._sellOrders.push_back(order); }
				}
				catch(boost::bad_lexical_cast &)
				{
					std::cout << "*** Corrupt Market Order, line " << line << " ***" << std::endl;
					return false;
				}
			}

			analyser_._sellOrders.sort(SortSellOrders());
			analyser_._buyOrders.sort(SortBuyOrders());

			return true;
		}

		bool LoadItemVolumes(MarketAnalyser & analyser_, std::string const & itemVolumesFile_)
		{
			analyser_._itemVolumes.clear();
			std::ifstream inFile(itemVolumesFile_.c_str());
			if(!inFile.is_open()){ return false; }

			std::string buf;

			int line = 0;
			while (!inFile.eof())
			{
				std::getline(inFile, buf); line++; if(inFile.eof()){ return true; }
				try
				{
					boost::tokenizer<boost::escaped_list_separator<char> > tok(buf);
					boost::tokenizer<boost::escaped_list_separator<char> >::iterator i=tok.begin();

					int itemId = boost::lexical_cast<int>((*i)); ++i;
					double itemVol = boost::lexical_cast<double>(*i); ++i;
					analyser_._itemVolumes[itemId] = itemVol;
				}
				catch (boost::bad_lexical_cast &)
				{
					std::cout << "Bad item volume record, line " << line << std::endl;
				}
			}

			return true;
		}

		bool ParseMarketOrder(MarketOrder & order_, std::string const & input_)
		{
			boost::tokenizer<boost::escaped_list_separator<char> > tok(input_);
			boost::tokenizer<boost::escaped_list_separator<char> >::iterator i=tok.begin();

			try
			{
				order_._price = boost::lexical_cast<double>(*i); ++i;
				order_._volRemaining = boost::lexical_cast<double>(*i); ++i;
				order_._typeId = boost::lexical_cast<int>(*i); ++i;
				order_._range = boost::lexical_cast<int>(*i); ++i;
				order_._orderId = boost::lexical_cast<int>(*i); ++i;
				order_._volEntered = boost::lexical_cast<int>(*i); ++i;
				order_._minVolume = boost::lexical_cast<int>(*i); ++i;
				order_._isBid = !((*i) == "False"); ++i;
				order_._issued = (*i); ++i;
				order_._duration = boost::lexical_cast<int>(*i); ++i;
				order_._stationId = boost::lexical_cast<int>(*i); ++i;
				order_._regionId = boost::lexical_cast<int>(*i); ++i;
				order_._systemId = boost::lexical_cast<int>(*i); ++i;
				order_._jumps = boost::lexical_cast<int>(*i); ++i;
			}
			catch (boost::bad_lexical_cast &) 
			{
				return false;
			}

			return true;
		}

		bool ParseSystem(Jumps & jumps_, std::string const & input_)
		{
			boost::tokenizer<boost::escaped_list_separator<char> > tok(input_);
			boost::tokenizer<boost::escaped_list_separator<char> >::iterator i=tok.begin();

			long system = boost::lexical_cast<double>(*i); ++i;
			double security = boost::lexical_cast<double>(*i); ++i;

			jumps_._systemSecurity[system] = security;

			return true;
		}

		bool ParseJumps(Jumps & jumps_, std::string const & input_)
		{
			boost::tokenizer<boost::escaped_list_separator<char> > tok(input_);
			boost::tokenizer<boost::escaped_list_separator<char> >::iterator i=tok.begin();

			long fromRegion = boost::lexical_cast<double>(*i); ++i;
			//long fromConst  = boost::lexical_cast<double>(*i); 
			++i;
			long fromSolar  = boost::lexical_cast<double>(*i); ++i;

			long toSolar    = boost::lexical_cast<double>(*i); ++i;
			//long toConst    = boost::lexical_cast<double>(*i); 
			++i;
			long toRegion   = boost::lexical_cast<double>(*i); ++i;

			jumps_.addStationConnection(fromRegion, fromSolar, toRegion, toSolar);

			return true;
		}

		bool IsSeperatorOfItems(char const c_)
		{
			std::string seperators("(,);");
			return seperators.find(c_) != std::string::npos;
		}

		bool ConvertDBItemsToCSV(std::string const & filenameIn_, std::string const & filenameOut_)
		{
			std::ifstream inFile(filenameIn_.c_str());
			if(!inFile.is_open()){ return false; }
			std::ofstream outFile(filenameOut_.c_str());
			if(!outFile.is_open()){ return false; }
			std::string buf;
			int line = 0;

			outFile.precision(2);

			try
			{		
				// Read each line in the file
				while (!inFile.eof())
				{
					std::getline(inFile, buf); line++;  if(inFile.eof()){ break; }
					if(std::string::npos != buf.find("VALUES("))
					{
						std::string t;

						if(std::string::npos == buf.find(");"))
						{
							do
							{
								std::getline(inFile, t); line++; if(inFile.eof()){ break; }
								buf += t;
							} while(std::string::npos == t.find(");"));
						}

						std::string::size_type firstQuote, secondQuote = 0;
						while( std::string::npos != (firstQuote = buf.find('\'')) )
						{
							if( (std::string::npos != (secondQuote = buf.find('\'', firstQuote + 1))) )
							{
								buf = buf.substr(0, firstQuote) + "0" + buf.substr(secondQuote + 1, buf.size() - (secondQuote + 1));
							}
						}

						//typeID,groupID,typeName,description,graphicID,radius,mass,volume,capacity,portionSize,raceID,basePrice,published,marketGroupID,chanceOfDuplicating)
						outFile.precision(2);
						std::vector<std::string> items;
						boost::algorithm::split(items, buf, IsSeperatorOfItems);

						std::vector<std::string>::iterator i = items.begin();

						if( (*i) != "VALUES" ){ std::cout << "Error reading items, line " << line << std::endl; break; } ++i;

						outFile << (*i) << ","; ++i; // typeId
						++i; //groupID
						++i; //typeName
						++i; //description
						++i; //graphicID
						++i; //radius
						++i; //mass
						outFile << std::fixed << boost::lexical_cast<double>(*i); ++i; // volume
						outFile << std::endl;
					}

				}
			}
			catch(boost::bad_lexical_cast &)
			{
				std::cout << "*** Corrupt InvTypesDB File, line " << line << " ***" << std::endl;
				return false;
			}

			return true;
		}

		bool IsSeperatorOfJumps(char const c_)
		{
			std::string seperators("(,);");
			return seperators.find(c_) != std::string::npos;
		}

		bool ConvertDBJumpsToCSV(std::string const & filenameIn_, std::string const & filenameOut_)
		{
			std::ifstream inFile(filenameIn_.c_str());
			if(!inFile.is_open()){ return false; }
			std::ofstream outFile(filenameOut_.c_str());
			if(!outFile.is_open()){ return false; }
			std::string buf;
			int line = 0;

			outFile.precision(2);

			try
			{		
				// Read each line in the file
				while (!inFile.eof())
				{
					std::getline(inFile, buf); line++; if(inFile.eof()){ break; }
					std::getline(inFile, buf); line++; if(inFile.eof()){ break; }
					std::getline(inFile, buf); line++; if(inFile.eof()){ break; }

					std::vector<std::string> items;
					boost::algorithm::split(items, buf, IsSeperatorOfJumps);

					std::vector<std::string>::iterator i = items.begin();

					if( (*i) != "VALUES" ){ break; } ++i;

					outFile << std::fixed << boost::lexical_cast<double>(*i) << ","; ++i;
					outFile << std::fixed << boost::lexical_cast<double>(*i) << ","; ++i;
					outFile << std::fixed << boost::lexical_cast<double>(*i) << ","; ++i;

					outFile << std::fixed << boost::lexical_cast<double>(*i) << ","; ++i;
					outFile << std::fixed << boost::lexical_cast<double>(*i) << ","; ++i;
					outFile << std::fixed << boost::lexical_cast<double>(*i) << "\n"; ++i;

					std::getline(inFile, buf); line++;
				}
			}
			catch(boost::bad_lexical_cast &)
			{
				std::cout << "*** Corrupt JumpsDB File, line " << line << " ***" << std::endl;
				return false;
			}

			return true;
		}

		bool ConvertDBSolarSystemToCSV(std::string const & filenameIn_, std::string const & filenameOut_)
		{
			std::ifstream inFile(filenameIn_.c_str());
			if(!inFile.is_open()){ return false; }
			std::ofstream outFile(filenameOut_.c_str());
			if(!outFile.is_open()){ return false; }
			std::string buf;
			int line = 0;

			outFile.precision(2);

			try
			{		
				// Read each line in the file
				while (!inFile.eof())
				{
					std::getline(inFile, buf); line++;  if(inFile.eof()){ break; }
					if(std::string::npos != buf.find("VALUES("))
					{
						std::string t;

						if(std::string::npos == buf.find(");"))
						{
							do
							{
								std::getline(inFile, t); line++; if(inFile.eof()){ break; }
								buf += t;
							} while(std::string::npos == t.find(");"));
						}

						std::string::size_type firstQuote, secondQuote = 0;
						while( std::string::npos != (firstQuote = buf.find('\'')) )
						{
							if( (std::string::npos != (secondQuote = buf.find('\'', firstQuote + 1))) )
							{
								buf = buf.substr(0, firstQuote) + "0" + buf.substr(secondQuote + 1, buf.size() - (secondQuote + 1));
							}
						}

						outFile.precision(2);
						std::vector<std::string> items;
						boost::algorithm::split(items, buf, IsSeperatorOfItems);

						std::vector<std::string>::iterator i = items.begin();

						if( (*i) != "VALUES" ){ std::cout << "Error reading items, line " << line << std::endl; break; } ++i;

						outFile << (*i) << ","; ++i; // regionID
						++i; //constellationID
						++i; //solarSystemID
						++i; //solarSystemName
						++i; //x
						++i; //y
						++i; //z
						++i; //xMin
						++i; //xMax
						++i; //yMin
						++i; //yMax
						++i; //zMin
						++i; //zMax
						++i; //luminosity
						++i; //border
						++i; //fringe
						++i; //corridor
						++i; //hub
						++i; //international
						++i; //regional
						++i; //constellation
						outFile << std::fixed << boost::lexical_cast<double>(*i); ++i; //security
						++i; //factionID
						++i; //radius
						++i; //sunTypeID
						++i; //securityClass
						++i; //allianceID

						outFile << std::endl;
					}

				}
			}
			catch(boost::bad_lexical_cast &)
			{
				std::cout << "*** Corrupt SolarSystemsDB File, line " << line << " ***" << std::endl;
				return false;
			}

			return true;
		}
	}
}
