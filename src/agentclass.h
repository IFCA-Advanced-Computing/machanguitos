//------------------------------------------------------------------------------
#ifndef AGENTCLASS_H
#define AGENTCLASS_H

#include "lua.hpp"
#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    constexpr const char * SCRIPT_AGENT_NAME = "Agent";

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

    //--------------------------------------------------------------------------
    inline lua_State * AgentClass::getLua() const{
        return m_L;
    }

    //--------------------------------------------------------------------------
    inline AgentInstance * AgentClass::createInstance(){
        return (new (std::nothrow) AgentInstance(this));
    }
}

//------------------------------------------------------------------------------
#endif//AGENTCLASS_H

//------------------------------------------------------------------------------
