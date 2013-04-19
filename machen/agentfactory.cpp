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
/** @file agentfactory.cpp
    @brief Agent::AgentFactory Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "agentfactory.hpp"
#include <cassert>
#include <boost/filesystem.hpp>
#include "lua.hpp"
#include "common/util.hpp"
#include "common/log.hpp"
#include "agentclass.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace boost::filesystem;
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    /** private name of AgentInstance objects in Lua tables.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_AGENTCLASS_OBJ = "__c";

    //--------------------------------------------------------------------------
    /** Define AgentClass.outVariables library function.

        This function sets the variables that should write out in each step of
        the simulation.

        @param L lua_State.
        @ingroup Agent
        @retval 0 No return values to Lua.
     */
    int ac_outVariables( lua_State *L ){
        lua_getglobal( L, SCRIPT_AGENTCLASS_NAME );
        lua_getfield( L, -1, SCRIPT_AGENTCLASS_OBJ );
        if( lua_islightuserdata( L, -1 ) ){
            AgentClass * aclass = (AgentClass*) lua_topointer( L, -1 );
            lua_pop( L, 2 );
            if( aclass ){
                auto n = lua_gettop( L );
                for( int i = 1 ; i <= n ; i++ ){
                    aclass->insertOutVariable( luaL_checkstring( L, i ) );
                }
            }else{
                luaL_error( L, "Invalid agent object" );
            }
        }

        return 0;
    }

    //--------------------------------------------------------------------------
    /** List of functions of AgentClass lua library for Agent files.
        @ingroup Agent
     */
    const luaL_Reg agentclasslib[] = {
        {"outVariables",  ac_outVariables},
        {nullptr, nullptr}
    };

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::createClass( const string & name ){
        auto el = m_classes.find( name );
        if( el != m_classes.end() ){
            return el->second;
        }

        auto dir = Engine::getDataDir();
        path filename = path(dir) /= (name + ".lua");

        if( !is_regular_file(filename) ){
            LOGE( "Not file for class '", name, "'" );
            return nullptr;
        }

        // Lua Initialization
        auto L = luaL_newstate();
        if( !L ){
            LOGE( "Can't create Lua State" );
            return nullptr;
        }

        lua_gc( L, LUA_GCSTOP, 0 );
        luaL_openlibs( L );
        lua_newtable( L );
        lua_setfield(L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
        lua_gc( L, LUA_GCRESTART, 0 );

        // create Agent class on Lua
        AgentClass * aclass = new (nothrow) AgentClass( L );
        if( !aclass ){
            LOGE( "Can't create agent class '", name, "' instance" );
            return nullptr;
        }

        // set functions
        luaL_register( L, SCRIPT_AGENTCLASS_NAME, agentclasslib );
        lua_pushlightuserdata( L, (void*)aclass );
        lua_setfield( L, -2, SCRIPT_AGENTCLASS_OBJ );
        // removes table
        lua_pop( L, 1 );

        // execute class file
        auto ret = luaL_dofile( L, filename.c_str() );
        if( !checkLuaReturn( L, ret ) ){
            delete aclass;
            return nullptr;
        }

        aclass->init();
        m_classes[name] = aclass;

        return aclass;
    }

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::getClass( const string & name ) const{
        auto el = m_classes.find( name );
        if( el != m_classes.end() ){
            return el->second;
        }
        return nullptr;
    }

}

//------------------------------------------------------------------------------
