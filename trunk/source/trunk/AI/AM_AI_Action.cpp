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

#include "AM_AI_Action.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		/*
		bool Action::operator==(Action const & action_) const
		{
		for (	std::map<std::string, Condition>::const_iterator i = action_._conditions.begin();
		i != action_._conditions.end(); ++i)
		{
		//if ()
		//{
		//}
		}

		return false;
		}
		*/

		bool Action::Condition::satisfiesPre(State const & state_) const
		{
			return _pre <= state_;
		}

		bool Action::Condition::satisfiesPre(Action::Condition const & condition_) const
		{
			return _pre <= condition_._pre;
		}

		std::vector<Action::Condition::ID> Action::validConditions(State const & state_)
		{
			std::vector<Condition::ID> satisfiedConditions;
			for (std::map<Condition::ID, Condition>::iterator i = _conditions.begin();
				i != _conditions.end(); ++i)
			{
				if((*i).second.satisfiesPre(state_) ){ satisfiedConditions.push_back((*i).first); }
			}
			return satisfiedConditions;
		}

//****************************************************************************//
	}
}