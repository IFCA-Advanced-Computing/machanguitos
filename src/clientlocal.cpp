//------------------------------------------------------------------------------
#include "clientlocal.h"

#include <cassert>

#include "agentfactory.h"

//------------------------------------------------------------------------------
namespace Engine{
    ClientLocal::ClientLocal(){
        // empty
    }

    ClientLocal::~ClientLocal(){
        // empty
    }

    bool ClientLocal::createClass( const std::string & name ){
        auto agentClass = Agent::AgentFactory::instance()->createClass( name );
        return (agentClass != nullptr);
    }
    
    void ClientLocal::createAgents( const std::string & name, int n ){
        auto agentClass = Agent::AgentFactory::instance()->getClass( name );
        assert( agentClass && "Class doen't exist" );
        // empty
    }

}

//------------------------------------------------------------------------------
