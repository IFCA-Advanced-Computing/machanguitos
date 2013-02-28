//------------------------------------------------------------------------------
#include "clientlocal.h"

#include <iostream>
#include <cassert>

#include "agentfactory.h"
#include "agentclass.h"
#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    ClientLocal::ClientLocal(){
        // empty
    }

    //--------------------------------------------------------------------------
    ClientLocal::~ClientLocal(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool ClientLocal::createClass( const string & name ){
        auto agentClass = Agent::AgentFactory::instance()->createClass( name );
        return (agentClass != nullptr);
    }

    //--------------------------------------------------------------------------
    void ClientLocal::createAgents( const string & name, int n ){
        auto agentClass = Agent::AgentFactory::instance()->getClass( name );
        if( agentClass ){
            for( int i = 0 ; i < n ; ++i ){
                Agent::AgentInstance * obj = agentClass->createInstance();
                if( obj ){
                    obj->init();
                    m_objects.push_back( obj );
                }else{
                    cerr << "can't create more instances of '"
                         << name << "'\n";
                    break;
                }
            }
        }else{
            cerr << "Class '" << name << "' doen't exist\n";
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
