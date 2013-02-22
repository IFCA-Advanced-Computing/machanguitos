//------------------------------------------------------------------------------
#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

#include <string>
#include <map>

struct lua_State;

//------------------------------------------------------------------------------
namespace Agent{
    class AgentClass;

    class AgentInstance {
    public:
        AgentInstance( AgentClass * c );
        void init();
        void update( const double delta );

        int pushData( lua_State * L, const std::string & key ) const;
        void newData( lua_State * L, const std::string & key );
    private:
        AgentClass * m_class{nullptr};
        std::map<std::string, double> m_vals;
    };
}

//------------------------------------------------------------------------------
#endif//AGENTINSTANCE_H

//------------------------------------------------------------------------------
