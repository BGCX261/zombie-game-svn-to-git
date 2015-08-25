#ifndef _AM_Corporation_hpp_
#define _AM_Corporation_hpp_

#include "AM_GameGeneral.hpp"

namespace AutoMatic
{
	namespace Corporation
	{
		extern StringValue const Name, Funds, Starships;
		extern StringValue const Insufficient_Funds;
		extern StringValue const Build_Starship;
	}
	
	/// A Corporation owns Starships.
	/** 
	***
	*/
	class CorporationFactory : public ObjectFactory
	{
	public:
		CorporationFactory(void);

		/// Set the name that will be given to the built Corporation object.
		void setName(std::string const & name_){ _name = name_; }
		/// Set the funds that will be given to the built Corporation object.
		void setFunds(float funds_){ _funds = funds_; }

		/// Builds a Corporation out of the supplied data.
		virtual ObjectPtr build(void);

	private:
		std::string _name;
		float _funds;

		ActionPtr _buildStarship;
	};
}

#endif //_AM_Corporation_hpp_