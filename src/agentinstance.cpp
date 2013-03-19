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
/** @file agentinstance.cpp
    @brief Agent::AgentInstance Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "agentinstance.h"

#include <iostream>
#include <cassert>

#include "lua.hpp"

#include "util.h"
#include "agentclass.h"
#include "datastore.h"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    AgentInstance::AgentInstance( AgentClass * c, AgentId && id )
        : m_class{c}, m_id{std::move(id)}
    {
        assert( m_class && "Invalid Agent Class" );
    }

    //--------------------------------------------------------------------------
    void AgentInstance::init(){
        auto L = m_class->getLua();
        if( L ){
            lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
            lua_getfield( L, -1, "init");
            if( lua_isfunction( L, -1 ) ){
                lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
                lua_pushstring( L, SCRIPT_AGENT_OBJ );
                lua_pushlightuserdata( L, (void*)this );
                lua_rawset( L, -3 );
                auto ret = lua_pcall( L, 1, 0, 0 );
                Util::checkLuaReturn( L, ret );
            }
        }
    }

    //--------------------------------------------------------------------------
    void AgentInstance::update( const double delta ){
        auto L = m_class->getLua();
        if( L ){
            lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
            lua_getfield( L, -1, "update");
            if( lua_isfunction( L, -1 ) ){
                lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );
                lua_pushstring( L, SCRIPT_AGENT_OBJ );
                lua_pushlightuserdata( L, (void*)this );
                lua_rawset( L, -3 );
                lua_pushnumber( L, delta );
                auto ret = lua_pcall( L, 2, 0, 0 );
                Util::checkLuaReturn( L, ret );
            }
        }
    }

    //--------------------------------------------------------------------------
    int AgentInstance::pushData( lua_State * L, const string & key ) const{
        const auto i = m_vals.find(key);
        if( i != m_vals.end() ){
            switch( i->second.getType() ){
            case ScriptValue::ValueType::NIL:
                lua_pushnil( L );
                break;

            case ScriptValue::ValueType::BOOLEAN:
                lua_pushboolean( L, i->second.getBoolean() );
                break;

            case ScriptValue::ValueType::NUMBER:
                lua_pushnumber( L, i->second.getNumber() );
                break;

            case ScriptValue::ValueType::STRING:
                lua_pushstring( L, i->second.getString().c_str() );
                break;

            default:
                lua_pushnil( L );
                break;
            }
        }else{
            lua_pushnil( L );
        }
        return 1;
    }

    //--------------------------------------------------------------------------
    void AgentInstance::newData( lua_State * L, const string & key ){
        auto ltype = lua_type( L, -2 );
        switch( ltype ){
        case LUA_TNIL:
            m_vals[key] = ScriptValue();
            break;
        case LUA_TNUMBER:
            m_vals[key] = ScriptValue(lua_tonumber( L, -2 ));
            break;
        case LUA_TBOOLEAN:
            m_vals[key] = ScriptValue(lua_toboolean( L, -2 )==1);
            break;
        case LUA_TSTRING:
            m_vals[key] = ScriptValue(lua_tostring( L, -2));
            break;
        default:
            cerr << "WARNING: type not implemented '" << lua_typename( L, ltype )
                 << "' on key '" << key << "'\n";
        }
    }

    //--------------------------------------------------------------------------
    void AgentInstance::outVars( const double t ) const{
        auto outKeys = m_class->getOutVars();

        auto db = IO::DataStore::instance();

        std::map<std::string, const ScriptValue *> ovars;
        for( const auto key: outKeys ){
            const auto variable = m_vals.find( key );
            if( variable != m_vals.end() ){
                ovars[key] = &variable->second;
            }
        }

        db->saveAgentInstance( t, m_id, ovars );
    }
}

//------------------------------------------------------------------------------
