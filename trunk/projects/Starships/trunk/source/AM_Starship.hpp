#ifndef _AM_Starship_hpp_
#define _AM_Starship_hpp_

#include "AM_GameGeneral.hpp"

namespace AutoMatic
{
	namespace Starship
	{
		extern StringValue const Cost;
		extern StringValue const Age;
		extern StringValue const TradeGoods;
		extern StringValue const Route;
	}

	/// A Starship transports TradeGoods between SolarSystems via TradeRoutes.
	/** Actions:
	***  - Load TradeGoods in a SolarSystem.
	***  - Unload TradeGoods in a SolarSystem.
	*/
	class StarshipFactory : public ObjectFactory
	{
	public:
		StarshipFactory(void);

		/// Set the cost of the Starship, to purchase.
		void setCost(float cost_){ _cost = cost_; }
		/// Set the name of the Corporation that owns this Starship.
		void setCorporationName(std::string const & corporationName_){ _corporationName = corporationName_; }

		virtual ObjectPtr build(void);

	private:
		std::string _corporationName;
		float _cost, _age;
	};
}

#endif //_AM_Starship_hpp_