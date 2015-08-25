#ifndef _AM_Eve_Jumps_hpp_
#define _AM_Eve_Jumps_hpp_

#include <boost/graph/visitors.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

namespace AutoMatic
{
	namespace Eve
	{
		class Jumps
		{
		public:
			typedef std::pair<long, long> StationName;

			typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS, 
				boost::property < boost::vertex_name_t, StationName > > Graph;

			typedef boost::property_map < Graph, boost::vertex_name_t >::type station_name_map_t;

			typedef boost::property_map < Graph, boost::edge_name_t >::type movie_name_map_t;

			typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;

			typedef std::map < StationName, Vertex > StationVertexMap;
			typedef std::map < Vertex, StationName > VertexStationMap;

			Jumps(void);
			~Jumps(void);

			std::pair<int, double> getJumps(StationName const & from_, StationName const & to_) const;
			
		private:
			friend bool ParseJumps(Jumps & jumps_, std::string const & input_);
			friend bool ParseSystem(Jumps & jumps_, std::string const & input_);
			friend bool LoadJumps(Jumps & jumps_, std::string const & jumpsFilename_, bool bailOnError_ );
			friend bool LoadSystems(Jumps & jumps_, std::string const & systemsFilename_, bool bailOnError_ );

			void insertStation(long region_, long solar_, Vertex & vertex_);

			void addStationConnection(long regionA_, long solarA_, long regionB_, long solarB_);

			Graph * _graph;
			StationVertexMap _stations;
			VertexStationMap _vertices;
			std::map<long, double> _systemSecurity;
		};
	}
}

#endif //_AM_Eve_Jumps_hpp_
