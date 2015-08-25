#ifndef _AM_GlobalStringTable_hpp_
#define _AM_GlobalStringTable_hpp_

#include <map>
#include <string>

namespace AutoMatic
{
	typedef unsigned int StringValue; 

	class GlobalStringTable
	{
	public:
		struct GlobalStringTableException : public std::exception
		{
		public:
			GlobalStringTableException(std::string const & string_)
				: std::exception("Could not find string in Global String Table.")
			{

			}

			std::string const & getString(void) const { return _string; }

		private:
			std::string _string;
		};

		GlobalStringTable(void)
			: _stringIndex(1)
		{}

		static StringValue const & Add(std::string const & string_);

		static StringValue const & Get(std::string const & string_);

	private:

		static GlobalStringTable & Singleton(void);

		StringValue _stringIndex;
		std::map<std::string, StringValue> _stringTable;

	};
}

#endif //_AM_GlobalStringTable_hpp_