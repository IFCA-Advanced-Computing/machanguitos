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

    //--------------------------------------------------------------------------
    AgentInstance::AgentInstance( AgentClass * c ) : m_class(c) {
        assert( m_class && "Invalid Agent Class" );
    }

    //--------------------------------------------------------------------------
    void AgentInstance::init(){
        auto L = m_class->getLua();
        assert( L && "Invalid Lua State" );
        lua_getfield( L, LUA_GLOBALSINDEX, "Agent" );
        lua_getfield( L, -1, "init");
        if( lua_isfunction( L, -1 ) ){
            lua_getfield( L, LUA_GLOBALSINDEX, "Agent" );
            lua_pushstring( L, "__obj" );
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
        lua_getfield( L, LUA_GLOBALSINDEX, "Agent" );
        lua_getfield( L, -1, "update");
        if( lua_isfunction( L, -1 ) ){
            lua_getfield( L, LUA_GLOBALSINDEX, "Agent" );
            lua_pushstring( L, "__obj" );
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
            lua_pushnumber( L, i->second );
        }else{
            lua_pushnil( L );
        }
        return 1;
    }

    //--------------------------------------------------------------------------
    void AgentInstance::newData( lua_State * L, const string & key ){
        auto ltype = lua_type( L, -2 );
        switch( ltype ){
        case LUA_TNUMBER:
            m_vals[key] = lua_tonumber( L, -2 );
            break;
        default:
            cerr << "WARNING: type not implemented " << lua_typename( L, ltype ) 
                 << " on key '" << key << "'\n";
        }
    }
}

//------------------------------------------------------------------------------
