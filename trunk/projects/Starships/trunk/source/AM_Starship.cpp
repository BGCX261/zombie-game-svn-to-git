#include "AM_Collections.hpp"

#include "AM_State.hpp"
#include "AM_Corporation.hpp"

#include "AM_Starship.hpp"

namespace AutoMatic
{
	namespace Starship
	{
		StringValue const Cost = GlobalStringTable::Add("Starship_Cost");
		StringValue const Age = GlobalStringTable::Add("Starship_Age");
		StringValue const TradeGoods  = GlobalStringTable::Add("Starship_TradeGoods");
		StringValue const Route = GlobalStringTable::Add("Starship_Route");
	}

	StarshipFactory::StarshipFactory(void)
		: _cost(0.0f), _age(0.0f), _corporationName("None")
	{		
	}

	ObjectPtr StarshipFactory::build(void)
	{
		ObjectPtr starship = NewObjectPtr();

		starship->setState(Corporation::Name, NewDataState<std::string>(_corporationName));
		starship->setState(Starship::Cost, NewDataState<float>(_cost));
		starship->setState(Starship::Age, NewDataState<float>(_age));
		starship->setState(Starship::TradeGoods, NewDataState< Set<ObjectPtr> >(Starship::TradeGoods));
		starship->setState(Starship::Route, NewDataState< List<ObjectPtr> >(Starship::Route));

		return starship;
	}
}
