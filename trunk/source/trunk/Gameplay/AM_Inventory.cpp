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

#include "Serialisation/AM_Serialisation.hpp"
#include "AM_Inventory.hpp"

namespace AutoMatic
{
//****************************************************************************//

	Inventory::~Inventory(void)
	{
		Iter i = _objects.begin();
		while( i != _objects.end() )
		{
			remove((*i));
			i = _objects.begin();
		} 
	}

	bool Inventory::add(Object * object_)
	{
		if(!object_){ return false; }

		if(InventoryItem * item = object_->component<InventoryItem>())
		{
			if(!canFit(item)){ return false; }

			item->setInventory(this);
			_objects.insert(object_);
			return true;
		}

		return false;
	}

	/// Remove an object from the inventory.
	/** \return True if the object was in the inventory.
	*/
	bool Inventory::remove(Object * object_)
	{
		if(!object_){ return false; }

		if(InventoryItem * item = object_->component<InventoryItem>())
		{
			if(item->_inventory != this){ return false; }
			item->setInventory(0);
			_objects.erase(object_);
			return true;
		}

		return true;
	}

//****************************************************************************//

	void InventoryItem::setInventory(Inventory * inventory_)
	{
		if(_inventory){ _inventory->remove(owner()); }
		_inventory = inventory_;
	}

	void InventoryItem::onRemove(void)
	{
		if(_inventory)
		{
			// TODO: Hack? It prevents an infinite recursion, because Inventory::remove() calls
				// InventoryItem::setInventory, which in turn calls Inventory::remove().
				// Setting the owner to null (which will happen anyway after onRemove() exits,
				// stops the recursion in Inventory::remove()
			Object * o = owner(); 
			owner(0);
			_inventory->remove(o);
		}
	}

//****************************************************************************//

	void RunInventorySaveLoadTest(void)
	{
		Object * inventoryObject = new Object;
		Object * inventoryItemObject = new Object;
		Inventory * inventory = new Inventory;
		InventoryItem * inventoryItem = new InventoryItem;

		inventoryObject->component(inventory);
		inventoryItemObject->component(inventoryItem);

		inventory->add(inventoryItemObject);

		{
			Serialise saveInventory("./tests/InventorySaveLoadTest.txt");
			saveInventory.out(inventoryObject);
		}
		
		delete inventoryObject; inventoryObject = 0;
		delete inventoryItemObject; inventoryItemObject = 0;

		delete inventory; inventory = 0;
		delete inventoryItem; inventoryItem = 0;

		{
			Deserialise loadInventory("./tests/InventorySaveLoadTest.txt");
			loadInventory.in(inventoryObject);
		}

		int itemCount = 0;
		inventory = inventoryObject->component<Inventory>();

		assert(inventoryObject);
		assert(inventory);
		assert(inventory->owner() == inventoryObject);
		assert(inventory->size() == 1);
		assert(inventory->begin() != inventory->end());

		for (Inventory::Iter i = inventory->begin(); i != inventory->end(); ++i)
		{
			itemCount++;

			inventoryItemObject = (*i);
			inventoryItem = inventoryItemObject->component<InventoryItem>();
			
			assert( inventoryItemObject );
			assert( inventoryItem );
			assert( inventoryItem->inventory() == inventory );
		}		

		assert(1 == itemCount);
		delete inventoryObject; inventoryObject = 0;
		delete inventoryItemObject; inventoryItemObject = 0;

		delete inventory; inventory = 0;
		delete inventoryItem; inventoryItem = 0;
	}

}
