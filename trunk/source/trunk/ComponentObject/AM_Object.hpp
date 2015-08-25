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

#ifndef _AM_Object_hpp_
#define _AM_Object_hpp_

#include <map>
#include <string>

#include "Serialisation/AM_SerialisationIncludes.hpp"

#include "AM_Component.hpp"
#include "AM_ComponentId.hpp"

namespace AutoMatic
{
	class Component;
	class ComponentId;
	class Object
	{
	public:
		Object(void)
		{}

		~Object(void);

		enum UpdateResult { Null, Disabled, Ok };
		
		/// Call update on a component of the desired type, if available.
		template<class ComponentType>
		UpdateResult update(float timestep_)
		{
			Component * c = _components[ComponentTypeId<ComponentType>::Id()];
			if(!c){ return Null; }
			if(!c->isEnabled()){ return Disabled; }
			
			c->update(timestep_);
			return Ok;
		}

		template<class ComponentType>
		void disable(void)
		{
			Component * c = _components[ComponentTypeId<ComponentType>::Id()];
			if(c){ c->disable(); }
		}

		template<class ComponentType>
		void enable(void)
		{
			Component * c = _components[ComponentTypeId<ComponentType>::Id()];
			if(c){ c->enable(); }
		}

		/// Retrieve a component of the specified type.
		/** \return Null if not in the object, otherwise, a pointer to the component.
		*/
		template<class ComponentType>
		ComponentType * component(void)
		{
			return static_cast<ComponentType *>(_components[ComponentTypeId<ComponentType>::Id()]);
		}

		/// Set a component for the Object.
		/** Remove any component of the ComponentType (including all base classes)
			* and replace with the new \a component_.
		*/
		template<class ComponentType>
		void component(ComponentType * component_)
		{
			remove<ComponentType>();

			add(component_);
		}

		/// Remove a component from the map.  Set to null in the map for all base types.
		template<class ComponentType>
		void remove(void)
		{
			return remove(_components[ComponentTypeId<ComponentType>::Id()]);
		}

		void remove(Component * component_);

	private:
		friend class boost::serialization::access;
		std::map<ComponentId const *, Component *> _components;

		void add(Component * component_)
		{
			if(0 != component_)
			{
				for (ComponentId::Iter i = component_->classId()->begin(); 
					i != component_->classId()->end(); ++i)
				{
					_components[*i] = component_;
				}

				component_->owner(this);	
				component_->onAttach();
			}
		}

		template<class Archive>
		void save(Archive & ar_, const unsigned int version_) const
		{
			int size = _components.size();

			ar_ & size;

			for (std::map<ComponentId const *, Component *>::const_iterator i = _components.begin();
					 i != _components.end(); ++i)
			{
				Component * c = (*i).second;
				ar_ & c;
			}
		}

		template<class Archive>
		void load(Archive & ar_, const unsigned int version_)
		{
			std::set<Component *> loadedComponents;
			std::set<Component *>::size_type loadedComponentsCount = 0;

			Component * c = 0;
			int size = 0;

			ar_ & size;
			for (int i = 0; i < size; ++i)
			{
				ar_ & c;
				
				loadedComponentsCount = loadedComponents.size();
				loadedComponents.insert(c);				
				
				if(loadedComponentsCount < loadedComponents.size())
				{
					add(c);
				}
			}
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
}

#endif //_AM_Object_hpp_