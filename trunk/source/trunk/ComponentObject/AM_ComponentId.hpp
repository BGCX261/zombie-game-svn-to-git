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

#ifndef _AM_ComponentId_hpp_
#define _AM_ComponentId_hpp_

#include <set>

namespace AutoMatic
{
	/// The ComponentId system does away with using strings as Id's
	/** Each ComponentId has a list if _ids.  This list refers to the
		* base classes of the Component (as well as containing this).
	*/
	class ComponentId
	{
	public:
		typedef std::set< ComponentId const * >::const_iterator Iter;

		ComponentId(void);
		~ComponentId(void);

		Iter begin(void) const { return _baseIds.begin(); }
		Iter end(void) const { return _baseIds.end(); }

	private:
		friend class Component;

		void inherit(ComponentId const * id_);
		std::set< ComponentId const * > _baseIds;
	};

	template<class ComponentType>
	class ComponentTypeId : public ComponentId
	{
	public:
		ComponentTypeId(void)
		{
		}

		static ComponentTypeId<ComponentType> * Id(void)
		{
			return &_Id;
		}

	private:
		static ComponentTypeId<ComponentType> _Id;
	};

	template<class ComponentType>
	ComponentTypeId<ComponentType> ComponentTypeId<ComponentType>::_Id;
}

#endif //_AM_ComponentId_hpp_