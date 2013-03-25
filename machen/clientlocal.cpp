/*******************************************************************************
Machanguitos is The Easiest Multi-Agent System in the world. Work done at The
Institute of Physics of Cantabria (IFCA).
Copyright (C) 2013  Luis Cabellos

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
/** @file clientlocal.cpp
    @brief Engine::ClientLocal Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "clientlocal.hpp"
#include <iostream>
#include <cassert>
#include "agentfactory.hpp"
#include "agentclass.hpp"
#include "agentinstance.hpp"
#include "datastore.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    ClientLocal::ClientLocal( const int id )
        : m_startTime{0}, m_totalTime{0}, m_ID{id}, m_nextID{0}
    {
        // empty
    }

    //--------------------------------------------------------------------------
    ClientLocal::~ClientLocal(){
        // empty
    }

    //--------------------------------------------------------------------------
    void ClientLocal::setDataDir( const std::string & path ){
        Agent::AgentFactory::instance()->setDatadir( path );
    }

    //--------------------------------------------------------------------------
    void ClientLocal::setDataStore( const std::string & name,
                                    const std::string & host, const uint16_t port )
    {
        auto db = IO::DataStore::instance();

        db->setDataStoreName( name );
        db->setDataStoreHost( host );
        db->setDataStorePort( port );
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
                auto obj = agentClass->createInstance( {m_ID, m_nextID} );
                if( obj ){
                    m_nextID += 1;
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

        m_totalTime += delta;

        for( auto obj: m_objects ){
            obj->outVars( m_startTime + m_totalTime );
        }
    }

    //--------------------------------------------------------------------------
    void ClientLocal::end(){
        // empty
    }
}

//------------------------------------------------------------------------------
