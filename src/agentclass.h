//------------------------------------------------------------------------------
#ifndef AGENTCLASS_H
#define AGENTCLASS_H

//------------------------------------------------------------------------------
#include "lua.hpp"
#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    /** Agent Table Name for Lua files.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_AGENT_NAME = "Agent";

    //--------------------------------------------------------------------------
    /** Class with Class definitions of Agents.
        @ingroup Agent
     */
    class AgentClass{
    public:
        /** Constructs the AgentClass object with a initialiced Lua State.
            @param L Lua State. It should be already initializated.
         */
        AgentClass( lua_State * L );
        ~AgentClass();

        /// Create a AgentInstance object of this AgentClass.
        AgentInstance * createInstance();
        /// Returns current Lua State for this AgentClass.
        lua_State * getLua() const;

    private:
        /// Lua State
        lua_State * m_L;
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
