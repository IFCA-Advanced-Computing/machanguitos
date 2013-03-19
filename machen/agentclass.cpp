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
#include <iostream>
#include "agentinstance.hpp"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace std;

    //--------------------------------------------------------------------------
    /** Define the indexing access of Agent intances metatable.
        @param L lua_State.
        @ingroup Agent
        @retval 0 No return values to Lua.
     */
    int agent_newindex( lua_State *L ){
        if( lua_isstring( L, -2 ) ){
            string key = lua_tostring( L, -2 );
            lua_pushstring( L, SCRIPT_AGENT_OBJ );
            lua_rawget( L, -4 );
            if( lua_islightuserdata( L, -1 ) ){
                AgentInstance * agent = (AgentInstance*) lua_topointer( L, -1 );
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
    /** Define the indexing assignment of Agent intances metatable.
        @param L lua_State.
        @ingroup Agent
        @retval 0 No return values to Lua.
     */
    int agent_index( lua_State *L ){
        if( lua_isstring( L, -1 ) ){
            string key = lua_tostring( L, -1 );
            lua_pushstring( L, SCRIPT_AGENT_OBJ );
            lua_rawget( L, -3 );
            if( lua_islightuserdata( L, -1 ) ){
                AgentInstance * agent = (AgentInstance*) lua_topointer( L, -1 );
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
    AgentClass::AgentClass( lua_State * L ) : m_L{L} {
        if( !m_L ){
            cerr << "AgentClass creation with invalid Lua State\n";
        }
    }

    //--------------------------------------------------------------------------
    void AgentClass::init(){
        if( m_L ){
            // set class methods for agent
            lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
            luaL_newmetatable( m_L, SCRIPT_AGENT_NAME );
            lua_pushstring( m_L, "__newindex");
            lua_pushcfunction( m_L, agent_newindex );
            lua_settable( m_L, -3 );
            lua_pushstring( m_L, "__index");
            lua_pushcfunction( m_L, agent_index );
            lua_settable( m_L, -3 );
            lua_setmetatable( m_L, -2 );
        }
    }

    //--------------------------------------------------------------------------
    AgentClass::~AgentClass(){
        if( m_L ){
            lua_close( m_L );
        }
    }

    //--------------------------------------------------------------------------
    void AgentClass::insertOutVariable( std::string && key ){
        m_outVars.insert( std::move(key) );
    }
}

//------------------------------------------------------------------------------
