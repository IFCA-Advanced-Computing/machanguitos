//------------------------------------------------------------------------------
#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include "singleton.h"

#include <string>

namespace Agent{
    class AgentClass;

    class AgentFactory : public Singleton<AgentFactory>{
    public:
        AgentClass * createClass( const std::string & name );
        AgentClass * getClass( const std::string & name ) const;
    private:
    };
}

//------------------------------------------------------------------------------
#endif//AGENTFACTORY_H

//------------------------------------------------------------------------------
