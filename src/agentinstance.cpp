//------------------------------------------------------------------------------
#include "agentinstance.h"

#include <iostream>
#include <cassert>

#include "lua.hpp"

#include "util.h"
#include "agentclass.h"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    AgentInstance::AgentInstance( AgentClass * c ) : m_class(c) {
        assert( m_class && "Invalid Agent Class" );
    }

    //--------------------------------------------------------------------------
    void AgentInstance::init(){
        auto L = m_class->getLua();
        assert( L && "Invalid Lua State" );
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

    //--------------------------------------------------------------------------
    void AgentInstance::update( const double delta ){
        auto L = m_class->getLua();
        assert( L && "Invalid Lua State" );
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
}

//------------------------------------------------------------------------------
