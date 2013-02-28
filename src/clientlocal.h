//------------------------------------------------------------------------------
#ifndef CLIENTLOCAL_H
#define CLIENTLOCAL_H

//------------------------------------------------------------------------------
#include <vector>

#include "client.h"

//------------------------------------------------------------------------------
namespace Agent{
    class AgentInstance;
}

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Client instance that runs in local proccess.
        @ingroup Engine
     */
    class ClientLocal : public Client{
    public:
        ClientLocal();
        virtual ~ClientLocal();

        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
        void runAgents( const double delta ) override;
        int numAgents() const override;
        void end() override;

    private:
        /// list of Agents in this Client instance.
        std::vector<Agent::AgentInstance *> m_objects;
    };

    //--------------------------------------------------------------------------
    inline int ClientLocal::numAgents() const{
        return m_objects.size();
    }
}

//------------------------------------------------------------------------------
#endif//CLIENTLOCAL_H

//------------------------------------------------------------------------------
