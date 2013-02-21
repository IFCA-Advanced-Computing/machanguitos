//------------------------------------------------------------------------------
#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

namespace Agent{
    class AgentClass;

    class AgentInstance {
    public:
        AgentInstance( AgentClass * c );
        void init();

    private:
        AgentClass * m_class{nullptr};
    };
}

//------------------------------------------------------------------------------
#endif//AGENTINSTANCE_H

//------------------------------------------------------------------------------
