//------------------------------------------------------------------------------
#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

//------------------------------------------------------------------------------
#include <string>
#include <map>

#include "scriptvalue.h"

//------------------------------------------------------------------------------
struct lua_State;

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    /** private name of AgentInstance objects in Lua tables.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_AGENT_OBJ = "__obj";

    //--------------------------------------------------------------------------
    class AgentClass;

    //--------------------------------------------------------------------------
    /** Instance of an AgentClass.
        @ingroup Agent
     */
    class AgentInstance {
    public:
        /** AgentInstance constructor. It should receive a AgentClass instance.
            @param c AgentClass instance of the Agent.
         */
        AgentInstance( AgentClass * c );
        /// initialize the Agent calling its 'init' function on Lua.
        void init();
        /** Update the Agent calling its 'update' fuction on Lua.
            @param delta time increment in seconds between updates.
         */
        void update( const double delta );

        /** Put the Agent variable on Lua stack.
            @param L Lua State.
            @param key name of the variable to get.
         */
        int pushData( lua_State * L, const std::string & key ) const;
        /** Save a Lua value in an Agent variable.
            @param L Lua State.
            @param key name of the variable to save.
         */
        void newData( lua_State * L, const std::string & key );
    private:
        /// AgentClass instance of this Agent.
        AgentClass * m_class;
        /// list of variables of the Agent.
        std::map<std::string, Util::ScriptValue> m_vals;
    };
}

//------------------------------------------------------------------------------
#endif//AGENTINSTANCE_H

//------------------------------------------------------------------------------
