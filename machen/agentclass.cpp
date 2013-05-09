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
/** @file agentclass.cpp
    @brief Agent::AgentClass Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "agentclass.hpp"
#include <cassert>
#include "agentinstance.hpp"
#include "common/util.hpp"
#include "dataserver.hpp"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace std;
    using namespace Util;

    constexpr const char * RASTER_OBJ = "__rt";

    //--------------------------------------------------------------------------
    /** Define the indexing access of Agent intances metatable.
        @param L lua_State.
        @ingroup Agent
        @retval 0 No return values to Lua.
     */
    int agent_newindex( lua_State *L ){
        if( lua_isstring( L, -2 ) ){
            string key = lua_tostring( L, -2 );
            lua_pushstring( L, SCRIPT_AGENT_OBJ );   // 1
            lua_rawget( L, -4 );                     // 1
            if( lua_islightuserdata( L, -1 ) ){
                auto agent = (AgentInstance*) lua_topointer( L, -1 );
                lua_pop( L, 1 );                     // 0
                if( agent ){
                    agent->newData( L, key );
                }else{
                    luaL_error( L, "Invalid agent object" );
                }
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    int raster_get( lua_State *L ){
        auto l = luaL_checknumber( L, -3 );
        auto x = luaL_checknumber( L, -2 );
        auto y = luaL_checknumber( L, -1 );
        lua_getfield( L, -4, RASTER_OBJ );        // 1
        if( lua_islightuserdata( L, -1 ) ){
            auto raster = (Data::Raster*) lua_topointer( L, -1 );
            lua_pop( L, 1 );                 // 0
            if( raster ){
                auto val = raster->getValue( l, x , y );
                lua_pushnumber( L, val );  // 1
                return 1;
            }else{
                luaL_error( L, "Invalid raster object" );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    int raster_set( lua_State *L ){
        auto x = luaL_checknumber( L, -3 );
        auto y = luaL_checknumber( L, -2 );
        lua_getfield( L, -4, RASTER_OBJ );        // 1
        if( lua_islightuserdata( L, -1 ) ){
            auto raster = (Data::Raster*) lua_topointer( L, -1 );
            lua_pop( L, 1 );                 // 0
            if( raster ){
            }else{
                luaL_error( L, "Invalid raster object" );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    /** Define the indexing assignment of Agent intances metatable.
        @param L lua_State.
        @ingroup Agent
        @retval 0 No return values to Lua.
     */
    int agent_index( lua_State *L ){
        if( lua_isstring( L, -1 ) ){
            string key = lua_tostring( L, -1 );
            lua_pushstring( L, SCRIPT_AGENT_OBJ );  // 1
            lua_rawget( L, -3 );                    // 1
            if( lua_islightuserdata( L, -1 ) ){
                AgentInstance * agent = (AgentInstance*) lua_topointer( L, -1 );
                lua_pop( L, 1 );                    // 0
                if( agent ){
                    return agent->pushData( L, key );
                }else{
                    luaL_error( L, "Invalid agent object" );
                }
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    /** Define the indexing access of raster table.
        @param L lua_State.
        @ingroup Agent
        @retval 0 No return values to Lua.
     */
    int raster_index( lua_State *L ){
        if( lua_isstring( L, -1 ) ){
            string key = lua_tostring( L, -1 );
            auto && ds = Engine::DataServer::instance();

            auto && layer = ds->getRaster( key );
            if( layer ){
                lua_newtable( L );                              // 1
                lua_pushstring( L, RASTER_OBJ );                // 2
                lua_pushlightuserdata( L, (void*)layer.get() ); // 3
                lua_rawset( L, -3 );                            // 1
                lua_pushstring( L, "get");                      // 2
                lua_pushcfunction( L, raster_get );             // 3
                lua_settable( L, -3 );                          // 1
                lua_pushstring( L, "set");                      // 2
                lua_pushcfunction( L, raster_set );             // 3
                lua_settable( L, -3 );                          // 1
                return 1;
            }else{
                luaL_error( L, ("Invalid raster layer '" + key + "'").c_str() );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    AgentClass::AgentClass( lua_State * L ) : m_L{L} {
        assert( m_L && "AgentClass creation with invalid Lua State" );
    }

    //--------------------------------------------------------------------------
    void AgentClass::init(){
        if( m_L ){
            // set class methods for agent
            lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME ); // 1
            luaL_newmetatable( m_L, SCRIPT_AGENT_NAME );              // 2
            lua_pushstring( m_L, "__newindex");                       // 3
            lua_pushcfunction( m_L, agent_newindex );                 // 4
            lua_settable( m_L, -3 );                                  // 2
            lua_pushstring( m_L, "__index");                          // 3
            lua_pushcfunction( m_L, agent_index );                    // 4
            lua_settable( m_L, -3 );                                  // 2
            lua_setmetatable( m_L, -2 );                              // 1
            lua_pop( m_L, 1 );                                        // 0
            // set "raster" data
            lua_newtable( m_L );                                      // 1
            luaL_newmetatable( m_L, "raster" );                       // 2
            lua_pushstring( m_L, "__index");                          // 3
            lua_pushcfunction( m_L, raster_index );                   // 4
            lua_settable( m_L, -3 );                                  // 2
            lua_setmetatable( m_L, -2 );                              // 1
            lua_setfield( m_L, LUA_GLOBALSINDEX, "raster");           // 0
        }
    }

    //--------------------------------------------------------------------------
    AgentClass::~AgentClass(){
        if( m_L ){
            lua_close( m_L );
        }
    }

    //--------------------------------------------------------------------------
    void AgentClass::insertOutVariable( string && key ){
        m_outVars.insert( move(key) );
    }
}

//------------------------------------------------------------------------------
