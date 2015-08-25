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

#ifndef _AM_AI_Output_hpp_
#define _AM_AI_Output_hpp_

namespace AutoMatic
{
	namespace AI
	{
		class Output
		{
		public:

			static Output & Out(void)
			{
				static Output out;
				return out;
			}

			class Endl
			{
			public:
				Endl(void){}
			};

			static Endl endl(void){ return Endl(); }
		};

		std::ostream & operator<<(std::ostream & out_, Output::Endl const & endl_);

		template<class T>
		Output & operator<<(Output & aiOut_, T const & t_)
		{
			//std::cout << t_;
			return aiOut_;
		}
	}
}

#endif //_AM_AI_Output_hpp_

