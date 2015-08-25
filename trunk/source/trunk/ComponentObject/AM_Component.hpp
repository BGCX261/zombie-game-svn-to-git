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

#ifndef _AM_Component_hpp_
#define _AM_Component_hpp_

#include "serialisation/AM_SerialisationIncludes.hpp"
#include "AM_ComponentId.hpp"

namespace AutoMatic
{
	class Object;

	/// 
	/** Usage: Derived components must call Id(this) in all constructors.
	*** This should be a flat hierarchy, DerivedComponents should not be inherited from.
	*/
	class Component
	{
	public:
		virtual ~Component(void);

		virtual void update(float const timestep_) 
		{}

		/// Set the owner of this component.
		/** This is virtual to allow derived components to do other things when the
			* owner is set.  E.g., add a component of their own, or have many owners.
		*/
		virtual void owner(Object * owner_){ _owner = owner_; }
		Object * owner(void) const { return _owner; }

		ComponentId const * const classId(void) const;

		/// Is the component enabled.  True by default.
		/** A disabled object won't be updated through object->update<>();
		*/
		bool isEnabled(void) const { return _enabled; }
		
		void enable(void){ _enabled = true; onEnable(); }
		void disable(void){ _enabled = false; onDisable(); }

	protected:
		friend class Object;

		/// Callback when added to an object.
		/** The new owner object will be set BEFORE this is called.
		*/
		virtual void onAttach(void){}

		/// Callback when removed from an object, or when the owner is destroyed.
		/** The owner will be set to null AFTER this is called.
		*/
		virtual void onRemove(void){}

		/// Callback when the component is enabled.
		virtual void onEnable(void){}

		/// Callback when the component is disabled.
		virtual void onDisable(void){}

		Component(void) 
			: _owner(0), _id(0), _enabled(true)
		{}

		/// 
		/** Every derived component must call this in its constructor, with 'this' as the argument.
		***
		*/
		template<class ComponentType>
		static void Id(ComponentType * component_)
		{
			ComponentId * prevId = component_->_id;
			component_->_id = ComponentTypeId<ComponentType>::Id();
			component_->_id->inherit(prevId);
		}

	private:
		friend class boost::serialization::access;

		Object * _owner; /**< The object that currently owns the component. **/
		ComponentId * _id; /**< Set by calling Id() **/
		bool _enabled;

		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & _enabled;
			// Note that _id and _owner are NOT serialised. 
			// The _owner will be set by reading in the object that owns this component.
			// The _id will be set automatically, and does not require serialisation.
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Component, "AutoMaticComponent")

#endif //_AM_Component_hpp_