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
#include <cassert>
#include "common/log.hpp"
#include "common/datastore.hpp"
#include "agentfactory.hpp"
#include "agentclass.hpp"
#include "agentinstance.hpp"
#include "dataserver.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;
    using namespace Util;

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
    void ClientLocal::createRaster( const Data::RasterNewData & nr ){
        LOGD( "ClientLocal::createRaster ", nr.key );
        auto && ds = Engine::DataServer::instance();
        switch( nr.rasterType ){
        case Data::RasterNewType::RNT_EMPTY:
            ds->createRasterProxy( nr.key, nr.layers, nr.w, nr.h,
                                   nr.x0, nr.x1, nr.y0, nr.y1, nr.d, nr.isFloat );
            break;

        case Data::RasterNewType::RNT_FILE:
            assert( false && "ClientLocal unimplemented" );
            break;
        }
    }

    //--------------------------------------------------------------------------
    bool ClientLocal::createClass( const string & name ){
        auto agentClass = Agent::AgentFactory::instance()->createClass( name );
        if( agentClass ){
            std::uniform_real_distribution<> dis;
            agentClass->setRandomSeed( dis(m_gen) );
        }
        return (agentClass != nullptr);
    }

    //--------------------------------------------------------------------------
    void ClientLocal::createAgents( const string & name, int n ){
        auto factory = Agent::AgentFactory::instance();
        auto agentClass = factory->getClass( name );
        if( agentClass ){
            for( int i = 0 ; i < n ; ++i ){
                auto obj = factory->createInstance( agentClass, Agent::AgentId{m_ID, m_nextID} );
                if( obj ){
                    m_nextID += 1;
                    obj->init();
                    m_objects.push_back( move(obj) );
                }else{
                    LOGE( "Can't create more instances of '", name, "'" );
                    break;
                }
            }
        }else{
            LOGE( "Class '", name, "' doen't exist" );
        }
    }

    //--------------------------------------------------------------------------
    void ClientLocal::runAgents( const double delta ){
        for( auto & obj: m_objects ){
            obj->update( delta );
        }

        m_totalTime += delta;

        auto db = Data::DataStore::instance();

        if( db->connect() ){
            for( auto & obj: m_objects ){
                obj->outVars( m_startTime + m_totalTime );
            }
            db->disconnect();
        }else{
            LOGE( "Can't save client agent instances" );
        }
    }

}

//------------------------------------------------------------------------------
