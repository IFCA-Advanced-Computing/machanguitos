//------------------------------------------------------------------------------
#ifndef CLIENTLOCAL_H
#define CLIENTLOCAL_H

#include <vector>

#include "client.h"

namespace Agent{
    class AgentInstance;
}

//------------------------------------------------------------------------------
namespace Engine{

    class ClientLocal : public Client{
    public:
        ClientLocal();
        virtual ~ClientLocal();

        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
        void runAgents( const double delta ) override;
        void end() override;

    private:
        std::vector<Agent::AgentInstance *> m_objects;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENTLOCAL_H

//------------------------------------------------------------------------------
