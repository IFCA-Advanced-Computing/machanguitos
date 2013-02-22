//------------------------------------------------------------------------------
#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include "singleton.h"

#include <string>
#include <map>

namespace Agent{
    class AgentClass;

    class AgentFactory : public Singleton<AgentFactory>{
    public:
        void setDatadir( const std::string & dir );
        AgentClass * createClass( const std::string & name );
        AgentClass * getClass( const std::string & name ) const;
    private:
        std::string m_dir{""};
        std::map<std::string, AgentClass *> m_classes;
    };
}

//------------------------------------------------------------------------------
#endif//AGENTFACTORY_H

//------------------------------------------------------------------------------
