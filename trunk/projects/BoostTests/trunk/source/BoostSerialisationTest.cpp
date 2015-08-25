#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/list.hpp>

class GPSPosition_Intrusive
{
public:
	GPSPosition_Intrusive(int degrees_ = 0, int minutes_ = 0, float seconds_ = 0.0f)
		:	_degrees(degrees_), _minutes(minutes_), _seconds(seconds_)
	{}

private:
	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar_, unsigned int const version_)
	{
		ar_ & _degrees;
		ar_ & _minutes;
		ar_ & _seconds;
	}

	int _degrees;
	int _minutes;
	float _seconds;
};

class GPSPosition_NonIntrusive
{
public:
	int degrees;
	int minutes;
	float seconds;
	GPSPosition_NonIntrusive(int d = 0, int m = 0, float s = 0.0f) :
	degrees(d), minutes(m), seconds(s)
	{

	}
};

namespace boost 
{
	namespace serialization 
	{

		template<class Archive>
		void serialize(Archive & ar, GPSPosition_NonIntrusive & g, const unsigned int version_)
		{
			ar & g.degrees;
			ar & g.minutes;
			ar & g.seconds;
		}

	} // namespace serialization
} // namespace boost

class BusStop
{
public:
	BusStop()
	{
	}
	virtual ~BusStop()
	{
	}

	BusStop(const GPSPosition_Intrusive & lat_, const GPSPosition_NonIntrusive & long_) 
		: _latitude(lat_), _longitude(long_)
	{}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar_, const unsigned int version_)
	{
		ar_ & _latitude;
		ar_ & _longitude;
	}
	GPSPosition_Intrusive _latitude;
	GPSPosition_NonIntrusive _longitude;
};

//BOOST_CLASS_EXPORT_GUID(BusStop, "BusStop")

class BusStopCorner : public BusStop
{
public:
	BusStopCorner()
	{
	}
	BusStopCorner(GPSPosition_Intrusive const & lat_, GPSPosition_NonIntrusive const & long_,
		std::string const & street1_, std::string const & street2_)
		: BusStop(lat_, long_), _street1(street1_), _street2(street2_)
	{
	}

	std::string _street1;
	std::string _street2;

private:

	virtual std::string description() const
	{
		return _street1 + " and " + _street2;
	}
};

namespace boost 
{
	namespace serialization 
	{

		template<class Archive>
		void serialize(Archive & ar, BusStopCorner & g, const unsigned int version_)
		{
				// serialize base class information
				ar & boost::serialization::base_object<BusStop>(g);
				ar & g._street1;
				ar & g._street2;
		}

	} // namespace serialization
} // namespace boost

BOOST_CLASS_EXPORT_GUID(BusStopCorner, "BusStopCorner")

class BusRoute
{
public:
	BusRoute(void){}

	BusRoute(BusStop * busStop1_, BusStop * busStop2_, BusStop * busStop3_)
	{
		//_stops[0] = busStop1_; _stops[1] = busStop2_;
		_stops.push_back(busStop1_);
		_stops.push_back(busStop2_);
		_stops.push_back(busStop3_);
	}

	~BusRoute(void)
	{
	}

private:
	friend class boost::serialization::access;
	std::list<BusStop *> _stops;

	template<class Archive>
	void serialize(Archive & ar_, const unsigned int version_)
	{
		ar_ & _stops;
	}
};

BOOST_CLASS_VERSION(BusRoute, 1) // This will set the version_ parameter.

void DeleteBusstops(std::list<BusStop *> & stops_)
{
	for (std::list<BusStop *>::iterator i = stops_.begin(); i != stops_.end(); ++i)
	{
		delete *i;
	}
	stops_.clear();
}

int main() 
{
	// Be aware that when reading in from an archive, pointers are new'd.
	std::list<BusStop *> allBusStops;

	std::string const filename = "GPS.dat";
	std::ofstream ofs(filename.c_str(), std::ios::binary);
	boost::archive::text_oarchive oa(ofs);
	GPSPosition_Intrusive gpsI(1,1,1.0f);
	GPSPosition_NonIntrusive gpsNI(2,2,2.0f);
	BusStop * bs = new BusStop(gpsI, gpsNI);
	BusStopCorner * bsc = new BusStopCorner(gpsI, gpsNI, "Street1", "Street2");
	BusStop * bs2 = new BusStop(gpsI, gpsNI);
	BusRoute br(bs, bsc, bs2);
	BusRoute const & brcp = br;
	std::list<BusStop *> const & allBusStopsRef = allBusStops;
	
	allBusStops.push_back(bs);
	allBusStops.push_back(bsc);
	allBusStops.push_back(bs2);

	oa << allBusStopsRef << brcp;
	ofs.close();

	DeleteBusstops(allBusStops);

	std::ifstream ifs(filename.c_str(),  std::ios::binary);
	boost::archive::text_iarchive ia(ifs);

	BusRoute nbr;
	ia >> allBusStops >> nbr;

	DeleteBusstops(allBusStops);

	return 0;
}
