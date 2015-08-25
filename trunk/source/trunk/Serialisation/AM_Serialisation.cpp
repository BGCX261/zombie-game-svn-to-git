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

#include <cassert>
#include "AM_Serialisation.hpp"

namespace AutoMatic
{
	class ExampleSerialisationBase
	{
	public:
		ExampleSerialisationBase(void)
			: _value1(0)
		{}

		virtual ~ExampleSerialisationBase(void)
		{}

		int const value1(void) const { return _value1; }
		void value1(int const value1_){ _value1 = value1_; }

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & _value1;
		}

		int _value1;
	};
}		
	
BOOST_CLASS_EXPORT_GUID(AutoMatic::ExampleSerialisationBase, "AutoMaticExampleSerialisationBase")

namespace AutoMatic
{
	class ExampleSerialisationDerived : public ExampleSerialisationBase
	{
	public:
		ExampleSerialisationDerived(void)
			: _value2(0)
		{}

		virtual ~ExampleSerialisationDerived(void)
		{}

		int const value2(void) const { return _value2; }
		void value2(int const value2_){ _value2 = value2_; }

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<ExampleSerialisationBase>(*this);
			ar_ & _value2;
		}

		int _value2;
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::ExampleSerialisationDerived, "AutoMaticExampleSerialisationDerived")

namespace AutoMatic
{
	void ExampleSerialisationBase_Should_Write_And_Read_A_Value1_Of_1(void)
	{
		ExampleSerialisationBase * base = new ExampleSerialisationBase;
		base->value1(1);
		
		{
			bool succeeded = false;
			Serialise write("./tests/ExampleSerialisation.txt");
			succeeded = write.out(base);
			assert(succeeded);
		}
		
		delete base; base = 0;

		{
			bool succeeded = false;
			Deserialise read("./tests/ExampleSerialisation.txt");
			succeeded = read.in(base);
			assert(succeeded);
		}

		assert(1 == base->value1());
		delete base;
	}

	void ExampleSerialisationDerived_Should_Write_And_Read_A_Value1_Of_1_And_A_Value2_Of_2(void)
	{
		ExampleSerialisationDerived * derived = new ExampleSerialisationDerived;
		derived->value1(1);
		derived->value2(2);

		{
			bool succeeded = false;
			Serialise write("./tests/ExampleSerialisation.txt");
			succeeded = write.out(derived);
			assert(succeeded);
		}

		delete derived; derived = 0;

		{
			bool succeeded = false;
			Deserialise read("./tests/ExampleSerialisation.txt");
			succeeded = read.in(derived);
			assert(succeeded);
		}

		assert(1 == derived->value1());
		assert(2 == derived->value2());
		delete derived;
	}

	void ExampleSerialisationDerived_Should_Write_And_Read_A_Value1_Of_1_And_A_Value2_Of_2_From_ExampleSerialisationBase_Pointer(void)
	{
		ExampleSerialisationDerived * derived = new ExampleSerialisationDerived;
		derived->value1(1);
		derived->value2(2);

		{
			ExampleSerialisationBase * base = derived;
			bool succeeded = false;
			Serialise write("./tests/ExampleSerialisation.txt");
			succeeded = write.out(base);
			assert(succeeded);
		}

		delete derived; derived = 0;

		{
			ExampleSerialisationBase * base = derived;
			bool succeeded = false;
			Deserialise read("./tests/ExampleSerialisation.txt");
			succeeded = read.in(base);
			assert(succeeded);

			derived = static_cast<ExampleSerialisationDerived *>(base);
		}

		assert(1 == derived->value1());
		assert(2 == derived->value2());
		delete derived;
	}

	void RunSerialisationTest(void)
	{
		ExampleSerialisationBase_Should_Write_And_Read_A_Value1_Of_1();
		ExampleSerialisationDerived_Should_Write_And_Read_A_Value1_Of_1_And_A_Value2_Of_2();
		ExampleSerialisationDerived_Should_Write_And_Read_A_Value1_Of_1_And_A_Value2_Of_2_From_ExampleSerialisationBase_Pointer();
	}
}
