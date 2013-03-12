/*******************************************************************************
Machanguitos is The Easiest Multi-Agent System in the world. Work done at The
Institute of Physics of Cantabria (IFCA).
Copyright (C) 2013  Luis Cabellos

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
//------------------------------------------------------------------------------
#include "configlib.h"

#include <iostream>
#include <cassert>

#include <boost/filesystem.hpp>

#include "lua.hpp"

#include "util.h"
#include "config.h"
#include "server.h"
#include "agentfactory.h"

//------------------------------------------------------------------------------
namespace Config{
    using namespace std;
    using namespace boost::filesystem;
    using namespace Util;

    //--------------------------------------------------------------------------
    int config_add_agent( lua_State *L ){
        auto name = luaL_checkstring( L, 1 );
        auto num = luaL_checknumber( L, 2 );
        auto server = Engine::Server::instance();
        server->addAgents( name, num );
        return 0;
    }

    //--------------------------------------------------------------------------
    int config_setvars( lua_State *L ){
        auto server = Engine::Server::instance();
        luaL_checktype( L, 1, LUA_TTABLE );
        // push first key
        lua_pushnil( L );
        while( lua_next( L, 1 ) != 0 ) {
            std::string key( luaL_checkstring( L, -2 ) );
            auto ltype = lua_type( L, -1 );
            switch( ltype ){
            case LUA_TNIL:
                server->insertConfig( key, ScriptValue() );
                break;
            case LUA_TNUMBER:
                server->insertConfig( key, ScriptValue(lua_tonumber( L, -1 )) );
                break;
            case LUA_TBOOLEAN:
                server->insertConfig( key, ScriptValue(lua_toboolean( L, -1)==1) );
                break;
            case LUA_TSTRING:
                server->insertConfig( key, ScriptValue(lua_tostring( L, -1)) );
                break;
            default:
                luaL_warn( L, "type not implemented '%s' on key '%s'",
                           lua_typename( L, ltype ), key.c_str() );
            }

            // removes 'value'; keeps 'key' for next iteration
            lua_pop( L, 1 );
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    const luaL_Reg configlib[] = {
        {"add_agent",   config_add_agent},
        {"setvars",   config_setvars},
        {nullptr, nullptr}
    };

    //--------------------------------------------------------------------------
    int openlib( lua_State *L ){
        // set functions
        luaL_register( L, "config", configlib );
        // Set constants
        lua_pushnumber( L, VERSION_MAJOR );
        lua_setfield( L, -2, "VERSION_MAJOR" );
        lua_pushnumber( L, VERSION_MINOR );
        lua_setfield( L, -2, "VERSION_MINOR" );
        lua_pushnumber( L, VERSION_RELEASE );
        lua_setfield( L, -2, "VERSION_RELEASE" );
        // removes table
        lua_pop( L, 1 );
        return 0;
    }

    //--------------------------------------------------------------------------
    bool setDataDir( const string & filename ){
        // check file
        if( !exists( filename ) || !is_regular_file( filename ) ){
            cerr << "Error: There is no config file " << filename << endl;
            return false;
        }

        // set directory with Agent classes
        path datadir(filename);
        datadir.remove_filename();
        Agent::AgentFactory::instance()->setDatadir( datadir.c_str() );
        
        return true;
    }

    //--------------------------------------------------------------------------
    bool load( const string & filename ){
        if( !setDataDir( filename ) ){
            return false;
        }

        // Lua Initialization
        bool is_ok{true};

        auto L = luaL_newstate();
        assert( L && "Can't create Lua State" );

        lua_gc(L, LUA_GCSTOP, 0);
        luaL_openlibs( L );
        Config::openlib( L );
        lua_gc(L, LUA_GCRESTART, 0);

        // execute config file
        auto ret = luaL_dofile( L, filename.c_str() );
        is_ok = Util::checkLuaReturn( L, ret );

        lua_close( L );

        return is_ok;
    }
}

//------------------------------------------------------------------------------
