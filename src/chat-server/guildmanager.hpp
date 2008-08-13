/*
 *  The Mana World Server
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World  is free software; you can redistribute  it and/or modify it
 *  under the terms of the GNU General  Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or any later version.
 *
 *  The Mana  World is  distributed in  the hope  that it  will be  useful, but
 *  WITHOUT ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should  have received a  copy of the  GNU General Public  License along
 *  with The Mana  World; if not, write to the  Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  $Id$
 */
#ifndef TMW_CHATSERVER_GUILDMANAGER_H
#define TMW_CHATSERVER_GUILDMANAGER_H

#include <list>
#include <string>
#include <vector>

class Guild;
class ChatClient;

/**
 * Guild manager takes care of creating, removing and modifying guilds.
 */
class GuildManager
{
    public:
        /**
         * Constructor.
         */
        GuildManager();

        /**
         * Destructor.
         */
        ~GuildManager();

        /**
         * Creates a guild.
         */
        Guild* createGuild(const std::string &name, const std::string &playerName);

        /**
         * Removes a guild.
         */
        void removeGuild(Guild *guild);

        /**
         * Adds a member to a guild.
         */
        void addGuildMember(Guild *guild, const std::string &playerName);

        /**
         * Removes a member from a guild.
         */
        void removeGuildMember(Guild *guild, const std::string &playerName);

        /**
         * Returns the guild with the given id. O(n)
         *
         * @todo <b>b_lindeijer:</b> Since this method is used so often, its
         *       efficiency should be improved, probably by storing the guilds
         *       in a map<int,Guild*> instead of list<Guild*>.
         *
         * @return the guild with the given id, or NULL if it doesn't exist
         */
        Guild *findById(short id);

        /**
         * Returns the guild with the given name. O(n)
         *
         * @return the guild with the given name, or NULL if it doesn't exist
         */
        Guild *findByName(const std::string &name);

        /**
         * Returns whether a guild exists.
         */
        bool doesExist(const std::string &name);

        /**
         * Return the guilds a character is in
         */
        std::vector<Guild*> getGuildsForPlayer(const std::string &name);

        /**
         * Inform guild members that a player has disconnected.
         */
        void disconnectPlayer(ChatClient* player);

        /**
         * Promote a guild member to higher level or
         * Demote a guild member to a lower level
         *
         * @return Returns 0 if successful, -1 otherwise
         */
        int changeMemberLevel(ChatClient *player, Guild *guild,
                               const std::string &name, int level);

    private:
        std::list<Guild*> mGuilds;
};

extern GuildManager *guildManager;

#endif
