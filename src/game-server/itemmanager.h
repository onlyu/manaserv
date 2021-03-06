/*
 *  The Mana Server
 *  Copyright (C) 2004-2010  The Mana World Development Team
 *
 *  This file is part of The Mana Server.
 *
 *  The Mana Server is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana Server is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana Server.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include "utils/xml.h"
#include "utils/string.h"

#include <string>
#include <map>
#include <vector>

class ItemClass;

struct EquipSlotInfo
{
    EquipSlotInfo():
        slotId(0), slotCapacity(0), visibleSlot(false)
    {}

    EquipSlotInfo(unsigned int id, const std::string &name,
                  unsigned int capacity, bool visible):
        slotId(id), slotName(name), slotCapacity(capacity), visibleSlot(visible)
    {}

    unsigned int slotId;
    std::string slotName;
    unsigned int slotCapacity;
    bool visibleSlot;
};


class ItemManager
{
    public:
        ItemManager(const std::string &itemFile, const std::string &equipFile) :
            mItemsFile(itemFile),
            mEquipSlotsFile(equipFile),
            mVisibleEquipSlotCount(0),
            mItemDatabaseVersion(0)
        {}

        ~ItemManager()
        { deinitialize(); }

        /**
         * Loads item reference file.
         */
        void initialize();

        /**
         * Reloads item reference file.
         */
        void reload();

        /**
         * Destroy item classes.
         */
        void deinitialize();

        /**
         * Gets the ItemClass having the given ID.
         */
        ItemClass *getItem(int itemId) const;

        /**
         * Gets the first item type with a specific name.
         * (comparison is case-insensitive).
         * Returns null when there is no item with such
         * a name.
         */
        ItemClass *getItemByName(const std::string &name) const;

        /**
         * Gets the version of the loaded item database.
         */
        unsigned int getDatabaseVersion() const;

        unsigned int getEquipSlotIdFromName(const std::string &name) const;

        unsigned int getEquipSlotCapacity(unsigned int id) const;

        unsigned int getVisibleEquipSlotCount() const
        { return mVisibleEquipSlotCount; }

        bool isEquipSlotVisible(unsigned int id) const;

    private:
        /** Loads the equip slots that a character has available to them. */
        void readEquipSlotsFile();

        /** Loads the main item database. */
        void readItemsFile();
        void readItemNode(xmlNodePtr itemNode);
        void readEquipNode(xmlNodePtr equipNode, ItemClass *item);
        void readEffectNode(xmlNodePtr effectNode, ItemClass *item);

        typedef std::map< int, ItemClass * > ItemClasses;
        ItemClasses mItemClasses; /**< Item reference */
        utils::NameMap<ItemClass*> mItemClassesByName;

        // Map an equip slot id with the equip slot info.
        typedef std::map< unsigned int, EquipSlotInfo* > EquipSlotsInfo;
        // Reference to the vector position of equipSlots
        typedef std::vector< unsigned int > VisibleEquipSlots;

        EquipSlotsInfo mEquipSlotsInfo;
        // Map a string (name of slot) with (str-id, max-per-equip-slot)
        // We only keep a pointer to it: The id map will take care of deletion.
        utils::NameMap<EquipSlotInfo* > mNamedEquipSlotsInfo;

        std::string mItemsFile;
        std::string mEquipSlotsFile;
        unsigned int mVisibleEquipSlotCount; // Cache

        /** Version of the loaded items database file.*/
        unsigned int mItemDatabaseVersion;
};

extern ItemManager *itemManager;

#endif
