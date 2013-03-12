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
#include "agentclass.h"

#include <iostream>

#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace std;

    //--------------------------------------------------------------------------
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
        if( m_L ){
            // set class methods for agent
            lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
            luaL_newmetatable( L, SCRIPT_AGENT_NAME );
            lua_pushstring(L, "__newindex");
            lua_pushcfunction( L, agent_newindex );
            lua_settable( L, -3 );
            lua_pushstring(L, "__index");
            lua_pushcfunction( L, agent_index );
            lua_settable( L, -3 );
            lua_setmetatable( L, -2 );
        }else{
            cerr << "AgentClass creation with invalid Lua State\n";
        }
    }

    //--------------------------------------------------------------------------
    AgentClass::~AgentClass(){
        if( m_L ){
            lua_close( m_L );
        }
    }
}

//------------------------------------------------------------------------------
