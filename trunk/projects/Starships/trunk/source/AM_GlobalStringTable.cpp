#include "AM_GlobalStringTable.hpp"

namespace AutoMatic
{
	GlobalStringTable & GlobalStringTable::Singleton(void)
	{
		static GlobalStringTable globalStringTable;
		return globalStringTable;
	}

	StringValue const & GlobalStringTable::Add(std::string const & string_)
	{		
		StringValue & stringValue = Singleton()._stringTable[string_];
		if(0 == stringValue){ stringValue = Singleton()._stringIndex++; }
		return stringValue;
	}

	StringValue const & GlobalStringTable::Get(std::string const & string_)
	{
		std::map<std::string, StringValue>::const_iterator i = 
			Singleton()._stringTable.find(string_);
		if(i == Singleton()._stringTable.end()){ throw GlobalStringTableException(string_); }
		return (*i).second;
	}
}
