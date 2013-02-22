//------------------------------------------------------------------------------
#ifndef AGENTCLASS_H
#define AGENTCLASS_H

#include "lua.hpp"

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    constexpr const char * SCRIPT_AGENT_NAME = "Agent";

    //--------------------------------------------------------------------------
    class AgentInstance;

    //--------------------------------------------------------------------------
    class AgentClass{
    public:
        AgentClass( lua_State * L );
        ~AgentClass();

        AgentInstance * createInstance();

        lua_State * getLua() const;

    private:
        lua_State * m_L{nullptr};
    };

    inline lua_State * AgentClass::getLua() const{
        return m_L;
    }
}

//------------------------------------------------------------------------------
#endif//AGENTCLASS_H

//------------------------------------------------------------------------------
