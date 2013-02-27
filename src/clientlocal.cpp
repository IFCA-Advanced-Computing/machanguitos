//------------------------------------------------------------------------------
#include "clientlocal.h"

#include <cassert>

#include "agentfactory.h"
#include "agentclass.h"
#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    ClientLocal::ClientLocal(){
        // empty
    }

    //--------------------------------------------------------------------------
    ClientLocal::~ClientLocal(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool ClientLocal::createClass( const std::string & name ){
        auto agentClass = Agent::AgentFactory::instance()->createClass( name );
        return (agentClass != nullptr);
    }

    //--------------------------------------------------------------------------
    void ClientLocal::createAgents( const std::string & name, int n ){
        auto agentClass = Agent::AgentFactory::instance()->getClass( name );
        assert( agentClass && "Class doen't exist" );
        for( int i = 0 ; i < n ; ++i ){
            Agent::AgentInstance * obj = agentClass->createInstance();
            if( obj != nullptr ){
                obj->init();
                m_objects.push_back( obj );
            }
        }
    }

    //--------------------------------------------------------------------------
    void ClientLocal::runAgents( const double delta ){
        for( auto obj: m_objects ){
            obj->update( delta );
        }
    }

    //--------------------------------------------------------------------------
    void ClientLocal::end(){
        // empty
    }
}

//------------------------------------------------------------------------------
