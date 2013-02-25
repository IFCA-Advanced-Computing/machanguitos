//------------------------------------------------------------------------------
#include "agentclass.h"

#include <iostream>
#include <cassert>

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
                assert( agent && "Invalid agent object" );
                agent->newData( L, key );
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
                assert( agent && "Invalid agent object" );
                return agent->pushData( L, key );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    AgentClass::AgentClass( lua_State * L ) : m_L{L} {
        assert( m_L && "Invalid AgentClass creation" );

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
    }

    //--------------------------------------------------------------------------
    AgentClass::~AgentClass(){
        assert( m_L && "Invalid AgentClass" );
        lua_close( m_L );
    }

    //--------------------------------------------------------------------------
    AgentInstance * AgentClass::createInstance(){
        AgentInstance * obj = new AgentInstance(this);
        assert( obj && "Can't create AgentInstance" );
        return obj;
    }
}

//------------------------------------------------------------------------------
