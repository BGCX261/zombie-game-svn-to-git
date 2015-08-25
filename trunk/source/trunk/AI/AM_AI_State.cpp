/*
Zombie Project.  A game.  With Zombies.
Copyright (C) 2007  Matthew Chudleigh

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "AM_AI_State.hpp"

namespace AutoMatic
{
	namespace AI
	{
		void State::clear(void)
		{
			_parts.clear();
		}

		void State::setValue(Part::Ptr & part_, Value::ID const & valueID_)
		{
			if(!part_ || !part_->getValue(valueID_)){ return; }
			_parts[part_] = part_->getValue(valueID_);
		}

		bool State::operator==(State const & state_) const
		{
			if(_parts.size() != state_._parts.size()){ return false; }
			return isEqual(*this, state_);
		}

		bool State::operator<=(State const & state_) const
		{
			if(_parts.size() > state_._parts.size()){ return false; }
			return isEqual(*this, state_);
		}

		bool State::operator<(State const & state_) const
		{
			if (_parts.size() < state_._parts.size()){ return true;}
			else if(_parts.size() == state_._parts.size())
			{
				std::map< Part::Ptr, Value::Ptr >::const_iterator i,j = state_._parts.begin();
				for (i = _parts.begin(); i != _parts.end(); ++i, ++j)
				{
					if( (*i).first < (*j).first ){ return true; }
					else if( (*i).first != (*j).first ){ return false; }
					if( (*i).second < (*j).second ){ return true; }
					else if( (*i).second != (*j).second ){ return false; }
				}
			}

			return false;
		}

		bool State::isEqual(State const & smaller_, State const & bigger_) const
		{
			std::map< Part::Ptr, Value::Ptr >::const_iterator i,j;
			for (i = smaller_._parts.begin(); i != smaller_._parts.end(); ++i)
			{
				j = bigger_._parts.find((*i).first);
				assert((*i).second.get() && (*i).second.get());
				if( j == bigger_._parts.end() || (*(*i).second) != (*(*j).second) ){ return false; }
			}
			return true;
		}

		State::operator bool(void) const 
		{ 
			if(_parts.size() <= 0){ return false; }
			std::map< Part::Ptr, Value::Ptr >::const_iterator i;
			for (i = _parts.begin(); i != _parts.end(); ++i)
			{
				if( !(*i).first || !(*i).second ){ return false; }
			}
			return true;
		}

		int State::countOfPartsEqual(State const & state_) const
		{
			int partsFound = 0;
			std::map< Part::Ptr, Value::Ptr >::const_iterator temp;
			for (std::map< Part::Ptr, Value::Ptr >::const_iterator i = state_._parts.begin();
				i != state_._parts.end(); ++i)
			{
				if ( (temp = _parts.find((*i).first)) != _parts.end() && (*temp).second == (*i).second)
				{
					partsFound++;
				}
			}

			return partsFound;
		}
	}
}