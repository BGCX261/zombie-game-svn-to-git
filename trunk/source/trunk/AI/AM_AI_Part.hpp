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

#ifndef _AM_AI_Part_hpp_
#define _AM_AI_Part_hpp_

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include "AM_AI_Value.hpp"

namespace AutoMatic
{
	namespace AI
	{
//****************************************************************************//

		/// Base class Part, holds a number of Value's.
		/** Each Part in the AI's state has an associated Value.
		*/
		class Part
		{
		public:
			typedef std::string ID;
			typedef boost::shared_ptr<Part> Ptr;

			Part(ID const & id_) : _id(id_) {}
			virtual ~Part(void){}

			/// Return a value with \a id_.  Will create a new value automatically.
			/** 
			*/
			Value::Ptr & getValue(Value::ID const & id_);

			ID const & id(void) const { return _id; }

		protected:
			ID _id;
			std::map<Value::ID, Value::Ptr> _values;

			/// Method used to create derived Value types.
			virtual Value * generateValue(Value::ID const & id_){ return new Value(id_); }
		};		

//****************************************************************************//
	}
}

#endif //_AM_AI_Part_hpp_