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

#ifndef _AM_Inventory_hpp_
#define _AM_Inventory_hpp_

#include <set>

#include "Serialisation/AM_SerialisationIncludes.hpp"
#include "ComponentObject/AM_Component.hpp"
#include "ComponentObject/AM_Object.hpp"

namespace AutoMatic
{
	class InventoryItem;
	/// 
	/** An for holding InventoryItem's and managing space.
	*** Example objects: Drawers, backpacks, belts, webbing, lockers.
	*/
	class Inventory : public Component
	{
	public:
		typedef std::set<Object *>::iterator Iter;

		Inventory(void) 
			: _parentInventory(0)
		{
			Component::Id(this);
		}

		virtual ~Inventory(void);

		/// Add an object to the inventory.
		/** \return True if the object is now in the inventory.
		*/
		bool add(Object * object_);

		/// Remove an object from the inventory.
		/** \return True if the object was in the inventory.
		*/
		bool remove(Object * object_);

		/// Can the item_ fit in this inventory?
		/** 
		***
		*/
		virtual bool canFit(InventoryItem * item_){ return true; }

		Iter begin(void){ return _objects.begin(); }
		Iter end(void){ return _objects.end(); }

		int size(void){ return _objects.size(); }

	private:
		Inventory * _parentInventory; /**< Only one allowed; An Inventory may NOT contain its parent or any ancestors. **/
		std::set<Object *> _objects;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Component>(*this);
			ar_ & _parentInventory;
			ar_ & _objects;
		}
	};
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::Inventory, "AutoMaticInventory")

namespace AutoMatic
{
	class Inventory;

	/// 
	/** An object with this component can be placed in an inventory (if it fits).
	***
	*/
	class InventoryItem : public Component
	{
	public:
		InventoryItem(void)
			: _inventory(0)
		{
			Component::Id(this);
		}

		Inventory * inventory(void){ return _inventory; }

	protected:

		virtual void onRemove(void);

	private:
		friend class Inventory;

		/// Set the inventory that this item belongs to.
		void setInventory(Inventory * inventory_);

		Inventory * _inventory; /**< The inventory the item currently belongs to. **/

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar_, const unsigned int version_)
		{
			ar_ & boost::serialization::base_object<Component>(*this);
			ar_ & _inventory; // TODO: Test this, ensure serialisation works like I think it does...
		}
	};

	void RunInventorySaveLoadTest(void);
}

BOOST_CLASS_EXPORT_GUID(AutoMatic::InventoryItem, "AutoMaticInventoryItem")

#endif //_AM_Inventory_hpp_