//------------------------------------------------------------------------------
#include "agentinstance.h"

#include <cassert>

#include "lua.hpp"

#include "util.h"
#include "agentclass.h"

//------------------------------------------------------------------------------
namespace Agent{
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
    int AgentInstance::pushData( lua_State * L, const std::string key ) const{
        lua_pushnumber( L, m_number );
        return 1;
    }

    //--------------------------------------------------------------------------
    void AgentInstance::newData( lua_State * L, const std::string key ){
        if( lua_isnumber( L, -2 ) ){
            m_number = lua_tonumber( L, -2 );
        }
    }
}

//------------------------------------------------------------------------------
