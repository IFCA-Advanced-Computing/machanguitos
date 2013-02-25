//------------------------------------------------------------------------------
#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

//------------------------------------------------------------------------------
#include <string>
#include <map>

#include "scriptvalue.h"

struct lua_State;

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    constexpr const char * SCRIPT_AGENT_OBJ = "__obj";

    //--------------------------------------------------------------------------
    class AgentClass;

    //--------------------------------------------------------------------------
    class AgentInstance {
    public:
        AgentInstance( AgentClass * c );
        void init();
        void update( const double delta );

        int pushData( lua_State * L, const std::string & key ) const;
        void newData( lua_State * L, const std::string & key );
    private:
        AgentClass * m_class{nullptr};
        std::map<std::string, Util::ScriptValue> m_vals;
    };
}

//------------------------------------------------------------------------------
#endif//AGENTINSTANCE_H

//------------------------------------------------------------------------------
