//------------------------------------------------------------------------------
#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

#include <string>

struct lua_State;

//------------------------------------------------------------------------------
namespace Agent{
    class AgentClass;

    class AgentInstance {
    public:
        AgentInstance( AgentClass * c );
        void init();

        int pushData( lua_State * L, const std::string key ) const;

        void newData( lua_State * L, const std::string key );
    private:
        AgentClass * m_class{nullptr};
        float m_number{0};
    };
}

//------------------------------------------------------------------------------
#endif//AGENTINSTANCE_H

//------------------------------------------------------------------------------
