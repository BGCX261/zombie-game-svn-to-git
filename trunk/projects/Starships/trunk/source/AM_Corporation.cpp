#include "AM_Collections.hpp"

#include "AM_Starship.hpp"
#include "AM_Corporation.hpp"

namespace AutoMatic
{
//****************************************************************************//
	StringValue const Corporation::Name = GlobalStringTable::Add("Corporation_Name");
	StringValue const Corporation::Funds = GlobalStringTable::Add("Corporation_Funds");
	StringValue const Corporation::Starships = GlobalStringTable::Add("Corporation_Starships");
	StringValue const Corporation::Insufficient_Funds = GlobalStringTable::Add("Corporation_Insufficient Funds");
	StringValue const Corporation::Build_Starship = GlobalStringTable::Add("Corporation_Build Starship");

	/// 
	/** Action for building a Starship for a Corporation.
	*** 
	*/
	class BuildStarship : public Action
	{
	public:
		virtual void run(Object & parent_, States const & arguments_)
		{
			float cost = DataState<float>::GetData(arguments_, Starship::Cost);
			float funds = parent_.getData<float>(Corporation::Funds);

			if(cost >= funds)
			{
				parent_.setData(Corporation::Funds, funds - cost);

				StarshipFactory starshipFactory;

				starshipFactory.setCorporationName( parent_.getData<std::string>(Corporation::Name) );
				starshipFactory.setCost(cost);

				ObjectPtr starship = starshipFactory.build();

				Set<ObjectPtr> & starships = parent_.getData< Set<ObjectPtr> >(Corporation::Starships);
				starships.push(starship, parent_);
			}
			else
			{
				// TODO: What is the best way to indicate failure of purchase?
				// A simple state change message? (Even though the state doesn't exist?)
				// No reason it won't work, it just seems a bit dodgy.
				// Also, Insufficient_Funds for what? Listeners only know this and 
				// what is in the Corporation state...
				// It just seems wrong to store such temporary information (i.e., the
				// action being called) in the state of the Corporation.
				parent_.sendStateChangeMessage(Corporation::Insufficient_Funds);
			}
		}
	};

//****************************************************************************//

	CorporationFactory::CorporationFactory(void)
		: _name("None"), _funds(0.0f)
		, _buildStarship(new BuildStarship)
	{		
		
	}

	ObjectPtr CorporationFactory::build(void)
	{
		ObjectPtr corporation = NewObjectPtr();

		corporation->setState(Corporation::Name, NewDataState<std::string>(_name));
		corporation->setState(Corporation::Funds, NewDataState<float>(_funds));
		corporation->setState(Corporation::Starships, NewDataState< Set<ObjectPtr> >(Set<ObjectPtr>(Corporation::Starships)));

		corporation->setAction(Corporation::Build_Starship, _buildStarship);
		return corporation;
	}

//****************************************************************************//
}
