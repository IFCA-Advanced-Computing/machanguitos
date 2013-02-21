//------------------------------------------------------------------------------
#include "agentclass.h"

#include <cassert>

//------------------------------------------------------------------------------
namespace Agent{

    //--------------------------------------------------------------------------
    int agent_newindex( lua_State *L ){
        cout << "agent newindex called\n";
        return 0;
    }

    //--------------------------------------------------------------------------
    int agent_index( lua_State *L ){
        cout << "agent index called\n";
        return 0;
    }

    //--------------------------------------------------------------------------
    AgentClass::AgentClass( lua_State * L ) : m_L{L} {
        assert( m_L && "Invalid AgentClass creation" );

        // set class methods for agent
        lua_getfield( L, LUA_GLOBALSINDEX, "Agent" );
        luaL_newmetatable( L, "Agent" );
        lua_pushstring(L, "__newindex");
        lua_pushcfunction( L, agent_newindex );
        lua_settable( L, -3 );
        lua_pushstring(L, "__index");
        lua_pushcfunction( L, agent_index );
        lua_settable( L, -3 );
        lua_setmetatable( L, -2 );
    }

    //--------------------------------------------------------------------------
    AgentClass::~AgentClass(){
        assert( m_L && "Invalid AgentClass" );
        lua_close( m_L );
    }

}

//------------------------------------------------------------------------------
