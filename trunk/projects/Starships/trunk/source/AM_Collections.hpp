#ifndef _AM_Collections_hpp_
#define _AM_Collections_hpp_

#include <set>
#include <list>

#include "AM_Object.hpp"

namespace AutoMatic
{
	template<class Data>
	class Set
	{
	public:
		Set(StringValue const & id_)
			: _id(id_)
		{}

		Set(Set const & set_)
			: _id(set_._id), _set(set_._set)
		{}

		void push(Data const & data_, Object & parent_)
		{
			_set.insert(data_);
			parent_.sendStateChangeMessage(_id);
		}

	private:
		StringValue const _id;
		std::set<Data> _set;
	};

	template<class Data>
	class List
	{
	public:
		List(StringValue const & id_)
			: _id(id_)
		{}

		List(List const & list_)
			: _id(list_._id), _list(list_._list)
		{}

		void push(Data const & data_, Object & parent_)
		{
			_list.push_back(data_);
			parent_.sendStateChangeMessage(_id);
		}

	private:
		StringValue const _id;
		std::list<Data> _list;
	};
}

#endif //_AM_Collections_hpp_