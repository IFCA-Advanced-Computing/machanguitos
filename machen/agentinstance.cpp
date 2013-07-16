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
#include "agentinstance.hpp"
#include <cassert>
#include "lua.hpp"
#include "common/util.hpp"
#include "common/log.hpp"
#include "common/datastore.hpp"
#include "agentclass.hpp"
#include "raster.hpp"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    AgentInstance::AgentInstance( std::shared_ptr<AgentClass> c, AgentId && id )
        : m_class{c}, m_id{move(id)}
    {
        assert( m_class && "Invalid Agent Class" );
    }

    //--------------------------------------------------------------------------
    void AgentInstance::init(){
        m_state = AgentState::AS_INIT;
        auto L = m_class->getLua();
        if( L ){
            lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );     // 1
            lua_getfield( L, -1, "init");                               // 2
            if( lua_isfunction( L, -1 ) ){
                lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME ); // 3
                lua_pushlightuserdata( L, (void*)this );                // 4
                lua_setglobal( L, SCRIPT_GLOBAL_AGENT_OBJ );            // 3
                auto ret = lua_pcall( L, 1, 0, 0 );                     // 1
                checkLuaReturn( L, ret );
                lua_pop( L, 1 );                                        // 0
            }else{
                lua_pop( L, 2 );                                        // 0
            }
        }
        m_state = AgentState::AS_NORMAL;
    }

    //--------------------------------------------------------------------------
    void AgentInstance::update( const double delta ){
        m_state = AgentState::AS_UPDATE;
        auto L = m_class->getLua();
        if( L ){
            lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );     // 1
            lua_getfield( L, -1, "update");                             // 2
            if( lua_isfunction( L, -1 ) ){
                lua_getfield( L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME ); // 3
                lua_pushlightuserdata( L, (void*)this );                // 4
                lua_setglobal( L, SCRIPT_GLOBAL_AGENT_OBJ );            // 3
                lua_pushnumber( L, delta );                             // 4
                auto ret = lua_pcall( L, 2, 0, 0 );                     // 1
                checkLuaReturn( L, ret );
                lua_pop( L, 1 );                                        // 0
            }else{
                lua_pop( L, 2 );                                        // 0
            }
        }
        m_state = AgentState::AS_NORMAL;
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
        auto ltype = lua_type( L, -1 );
        switch( ltype ){
        case LUA_TNIL:
            m_vals[key] = ScriptValue();
            break;
        case LUA_TNUMBER:
            m_vals[key] = ScriptValue(lua_tonumber( L, -1 ));
            break;
        case LUA_TBOOLEAN:
            m_vals[key] = ScriptValue(lua_toboolean( L, -1 )==1);
            break;
        case LUA_TSTRING:
            m_vals[key] = ScriptValue(lua_tostring( L, -1));
            break;
        default:
            LOGW( "Lua type not implemented '", lua_typename( L, ltype ),
                  "' on key '", key, "'" );
        }
    }

    //--------------------------------------------------------------------------
    void AgentInstance::newMessage( string && msg ){
        LOGW( "IO : ", msg );
    }

    //--------------------------------------------------------------------------
    void AgentInstance::outVars( const double t ) const{
        auto outKeys = m_class->getOutVars();

        auto db = IO::DataStore::instance();

        map<string, const ScriptValue *> ovars;
        for( const auto& key: outKeys ){
            const auto variable = m_vals.find( key );
            if( variable != m_vals.end() ){
                ovars[key] = &variable->second;
            }
        }

        db->saveAgentInstance( t, m_id, ovars );
    }

    //--------------------------------------------------------------------------
    double AgentInstance::getRasterValue( Data::Raster & raster, int l, double x, double y ){
        auto val = raster.getValue( l, x, y );
        if( raster.validPosition( x, y ) ){
            auto pos = raster.getPosition( x, y );
            insertCache( raster.key, l, get<0>(pos), get<1>(pos), val );
        }
        return val;
    }

    //--------------------------------------------------------------------------
    bool AgentInstance::setRasterValue( Data::Raster & raster, int l,
                                        double x, double y, double v )
    {
        if( raster.validPosition( x, y ) ){
            auto pos = raster.getPosition( x, y );
            auto it = findCache( raster.key, l, get<0>(pos), get<1>(pos) );
            if( get<0>(it) ){
                auto old = get<1>(it);
                return raster.updateValue( l, x, y, old, v );
            }else{
                raster.setValue( l, x, y, v );
            }
        }
        return true;
    }

    //--------------------------------------------------------------------------
    void AgentInstance::insertCache( string id, int l, int i, int j, double v ){
        auto & rmap = m_cache[ id ];
        rmap[ {l,i,j} ] = v;
    }

    //--------------------------------------------------------------------------
    tuple<bool,double> AgentInstance::findCache( string id, int l, int i, int j ){
        const auto it = m_cache.find( id );
        if( it != end(m_cache) ){
            const auto jt = it->second.find( {l,i,j} );
            if( jt != end(it->second) ){
                return make_tuple( true, jt->second );
            }
        }

        return make_tuple( false, 0 );
    }
}

//------------------------------------------------------------------------------
