#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "AM_Eve_Jumps.hpp"

namespace AutoMatic
{
	namespace Eve
	{
		template <typename DistanceMap>
		class JumpsToStationRecorder : public boost::default_bfs_visitor
		{
		public:
			JumpsToStationRecorder(DistanceMap dist, 
				std::map<long, double> const & security_, Jumps::VertexStationMap const & vertices_) 
				: d(dist) , _security(security_), _vertices(vertices_)
			{ }

			template <typename Edge, typename Graph>
			void tree_edge(Edge e, const Graph& g) const
			{
				typename boost::graph_traits<Graph>::vertex_descriptor
					u = boost::source(e, g), v = boost::target(e, g);

				d[v].first = d[u].first + 1;

				//std::ofstream debugSecurity("debugsecurity.txt");

				//debugSecurity << "Searching for u,v in vertices" << std::endl;

				Jumps::VertexStationMap::const_iterator uIter = _vertices.find(u);
				Jumps::VertexStationMap::const_iterator vIter = _vertices.find(v);

				std::map<long, double>::const_iterator sUIter, sVIter;
				double uSec = -2.0, vSec = -2.0;

				if(uIter != _vertices.end())
				{
					//debugSecurity << "Found uIter" << std::endl;
					sUIter = _security.find((*uIter).second.second);
					if(sUIter != _security.end())
					{ 
						uSec = (*sUIter).second; 
						//debugSecurity << "Found uSec: " << uSec << std::endl;
					}
				}

				if(vIter != _vertices.end())
				{
					//debugSecurity << "Found vIter: " << uSec << std::endl;
					sVIter = _security.find((*vIter).second.second);
					if(sVIter != _security.end())
					{ 
						vSec = (*sVIter).second; 
						//debugSecurity << "Found vSec: " << uSec << std::endl;
					}
				}				

				if(uSec <= -2.0)
				{ 
					uSec = vSec; 
					//debugSecurity << "uSec <= 2.0, now " << vSec << std::endl;
				}
				if(vSec <= -2.0)
				{ 
					vSec = uSec; 
					//debugSecurity << "vSec <= 2.0, now " << uSec << std::endl; 
				}

				d[v].second = std::min(uSec, vSec);

				//debugSecurity << "min is " << d[v].second << std::endl;
			}

		private:
			DistanceMap d;
			std::map<long, double> const & _security;
			Jumps::VertexStationMap const & _vertices;
		};

		// Convenience function
		template < typename DistanceMap >
		JumpsToStationRecorder<DistanceMap>
			RecordJumpsToStation(DistanceMap d, 
			std::map<long, double> const & security_, Jumps::VertexStationMap const & vertices_)
		{
			return JumpsToStationRecorder < DistanceMap > (d, security_, vertices_);
		}

		Jumps::Jumps(void)
			: _graph(0)
		{}

		Jumps::~Jumps(void)
		{
			delete _graph;
		}

		std::pair<int, double> Jumps::getJumps(StationName const & from_, StationName const & to_) const
		{
			if(!_graph){ return std::pair<int,double>(-1, 0.0); }
			std::vector < std::pair<int,double> > stationJumps( boost::num_vertices(*_graph));

			StationVertexMap::const_iterator toIter = _stations.find(to_);
			StationVertexMap::const_iterator fromIter = _stations.find(from_);
			if(toIter == _stations.end()){ return std::pair<int,double>(-1, 0.0); }
			if(fromIter == _stations.end()){ return std::pair<int,double>(-1, 0.0); }
			Vertex to = (*toIter).second;
			Vertex from = (*fromIter).second;

			stationJumps[to].first = 0;
			stationJumps[to].second = 0.0;

			boost::breadth_first_search(*_graph, to, boost::visitor(RecordJumpsToStation(&stationJumps[0], _systemSecurity, _vertices)));

			return stationJumps[from];
		}

		void Jumps::insertStation(long region_, long solar_, Vertex & vertex_)
		{
			StationName stationName(region_, solar_);
			StationVertexMap::iterator pos;

			bool inserted = false;
			boost::tie(pos, inserted) = _stations.insert(std::make_pair(stationName, Vertex()));

			if (inserted) 
			{
				vertex_ = boost::add_vertex(*_graph);
				pos->second = vertex_;
				_vertices[vertex_] = stationName;
			} else {
				vertex_ = pos->second;
			}
		}

		void Jumps::addStationConnection(long regionA_, long solarA_, long regionB_, long solarB_)
		{
			if(!_graph){ return; }

			Jumps::Vertex u, v;
			// Create graph vertices
			insertStation(regionA_, solarA_, u);
			insertStation(regionB_, solarB_, v);

			// Create graph edges
			boost::add_edge(u, v, *_graph);
		}
	}
}

int TestDjikstra(int, char *[])
{
	using namespace boost;

	typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > > graph_t;
	typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
	typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
	typedef std::pair<int, int> Edge;

	const int num_nodes = 5;
	enum nodes { A, B, C, D, E };
	char name[] = "ABCDE";
	Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
		Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
	};
	int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
	int num_arcs = sizeof(edge_array) / sizeof(Edge);

	graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor s = vertex(A, g);

	dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));

	std::cout << "distances and parents:" << std::endl;
	graph_traits < graph_t >::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
		std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
			endl;
	}
	std::cout << std::endl;

	std::ofstream dot_file("figs/dijkstra-eg.dot");

	dot_file << "digraph D {\n"
		<< "  rankdir=LR\n"
		<< "  size=\"4,3\"\n"
		<< "  ratio=\"fill\"\n"
		<< "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

	graph_traits < graph_t >::edge_iterator ei, ei_end;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
		graph_traits < graph_t >::edge_descriptor e = *ei;
		graph_traits < graph_t >::vertex_descriptor
			u = source(e, g), v = target(e, g);
		dot_file << name[u] << " -> " << name[v]
		<< "[label=\"" << get(weightmap, e) << "\"";
		if (p[v] == u)
			dot_file << ", color=\"black\"";
		else
			dot_file << ", color=\"grey\"";
		dot_file << "]";
	}
	dot_file << "}";
	return EXIT_SUCCESS;
}
