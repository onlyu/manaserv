/*
 *  The Mana World Server
 *  Copyright 2007 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <cassert>

#include "luascript.hpp"

#include "game-server/being.hpp"

#include "utils/logger.h"

LuaScript::~LuaScript()
{
    lua_close(mState);
}

void LuaScript::prepare(std::string const &name)
{
    assert(nbArgs == -1);
    lua_getglobal(mState, name.c_str());
    nbArgs = 0;
    mCurFunction = name;
}

void LuaScript::push(int v)
{
    assert(nbArgs >= 0);
    lua_pushinteger(mState, v);
    ++nbArgs;
}

void LuaScript::push(std::string const &v)
{
    assert(nbArgs >= 0);
    lua_pushstring(mState, v.c_str());
    ++nbArgs;
}

void LuaScript::push(Thing *v)
{
    assert(nbArgs >= 0);
    lua_pushlightuserdata(mState, v);
    ++nbArgs;
}

int LuaScript::execute()
{
    assert(nbArgs >= 0);
    int res = lua_pcall(mState, nbArgs, 1, 0);
    nbArgs = -1;
    if (res || !(lua_isnil(mState, 1) || lua_isnumber(mState, 1)))
    {
        char const *s = lua_tostring(mState, 1);

        LOG_WARN("Lua Script Error" << std::endl
                 << "     Script  : " << mScriptFile << std::endl
                 << "     Function: " << mCurFunction << std::endl
                 << "     Error   : " << (s ? s : "") << std::endl);
        lua_pop(mState, 1);
        return 0;
    }
    res = lua_tointeger(mState, 1);
    lua_pop(mState, 1);
    return res;
    mCurFunction = "";
}

void LuaScript::load(char const *prog)
{
    int res = luaL_loadstring(mState, prog);

    if (res == LUA_ERRSYNTAX)
    {
        LOG_ERROR("Syntax error while loading Lua script.");
        return;
    }

    // A Lua chunk is like a function, so "execute" it in order to initialize
    // it.
    res = lua_pcall(mState, 0, 0, 0);
    if (res)
    {
        LOG_ERROR("Failure while initializing Lua script: "
                  << lua_tostring(mState, -1));
        lua_settop(mState, 0);
        return;
    }
}

void LuaScript::processDeathEvent(Being *being)
{
    prepare("death_notification");
    push(being);
    //TODO: get and push a list of creatures who contributed to killing the
    //      being. This might be very interesting for scripting quests.
    execute();

    being->removeListener(getScriptDeathListener());
}

/**
 * Called when the server has recovered the value of a quest variable.
 */
void LuaScript::getQuestCallback(Character *q, std::string const &name,
                                 std::string const &value, void *data)
{
    LuaScript *s = static_cast< LuaScript * >(data);
    assert(s->nbArgs == -1);
    lua_getglobal(s->mState, "quest_reply");
    lua_pushlightuserdata(s->mState, q);
    lua_pushstring(s->mState, name.c_str());
    lua_pushstring(s->mState, value.c_str());
    s->nbArgs = 3;
    s->execute();
}

/**
 * Called when the server has recovered the post for a user
 */
void LuaScript::getPostCallback(Character *q, std::string const &sender,
                                std::string const &letter, void *data)
{
    // get the script
    LuaScript *s = static_cast<LuaScript*>(data);
    assert(s->nbArgs == -1);
    lua_getglobal(s->mState, "post_reply");
    lua_pushlightuserdata(s->mState, q);
    lua_pushstring(s->mState, sender.c_str());
    lua_pushstring(s->mState, letter.c_str());
    s->nbArgs = 3;
    s->execute();
}