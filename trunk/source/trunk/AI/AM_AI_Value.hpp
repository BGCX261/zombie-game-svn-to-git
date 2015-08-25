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

#ifndef _AM_AI_Value_hpp_
#define _AM_AI_Value_hpp_

#include <string>
#include <boost/shared_ptr.hpp>

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		/// Base class Value, defines a possible value for the Logic class.
		/** An AI has one current Value for each Logic class in its state.
		* Derived classes provide systems for finding which Value an AI currently
		* has.  
		* \sa Fuzzy::Set
		*/
		class Value
		{
		public:
			typedef std::string ID;
			typedef boost::shared_ptr<Value> Ptr;

			Value(ID const & id_) : _id(id_) {}
			virtual ~Value(void){}

			ID const & id(void) const { return _id; }

			bool operator!=(Value const & value_)
			{
				return !(*this == value_);
			}

			bool operator==(Value const & value_)
			{
				return _id == value_._id;
			}

		protected:
			ID _id;
		};

//****************************************************************************//
	}
}

#endif //_AM_AI_Value_hpp_