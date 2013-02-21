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
            auto ret = lua_pcall( L, 1, 0, 0 );
            Util::checkLuaReturn( L, ret );
        }
    }

}

//------------------------------------------------------------------------------
