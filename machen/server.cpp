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
/** @file server.cpp
    @brief Engine::Server Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "server.hpp"
#include <iostream>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <mpi.h>
#include "common/datastore.hpp"
#include "config.h"
#include "agentfactory.hpp"
#include "clientlocal.hpp"
#include "clientremote.hpp"
#include "mpidefs.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    void Server::setDatadir( const string & filename ){
        boost::filesystem::path datadir( filename );
        datadir.remove_filename();
        m_datadir = datadir.c_str();
    }

    //--------------------------------------------------------------------------
    void Server::createClients( const int nprocs ){
        if( nprocs <= 2 ){
            m_clients.emplace_back( new ClientLocal( 0 ) );
        }

        for( int i = 2 ; i < nprocs ; ++i ){
            m_clients.emplace_back( new ClientRemote( i ) );
        }
    }

    //--------------------------------------------------------------------------
    void Server::addAgents( const string & name, const unsigned n ){
        if( name.length() > MAX_CLASS_NAME ){
            cerr << "WARNING: Class name '" << name << "' too long\n";
        }else{
            m_numAgents[name] = m_numAgents[name] + n;
        }
    }

    //--------------------------------------------------------------------------
    void Server::createRaster( const string & key, int w, int h,
                       double x0, double x1, double y0, double y1 ){
        if( key.length() > MAX_CLASS_NAME ){
            cerr << "WARNING: raster name '" << key << "' too long\n";
        }else{
            cout  << "creating raster named '" << key << "'\n";
            m_newRaster.emplace_front( key, w, h, x0, x1, y0, y1 );
        }
    }

    //--------------------------------------------------------------------------
    bool Server::initialize(){
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );

        createClients( nprocs );

        m_comm = createClientsComm();

        m_dataServer = DataServer::instance();

        auto db = IO::DataStore::instance();
        auto host = getConfigString( "dbhost", IO::DataStore::DEFAULT_HOSTNAME );
        auto port = getConfigInt( "dbport", IO::DataStore::DEFAULT_PORT );
        db->setDataStoreHost( host );
        db->setDataStorePort( port );

        auto name = db->mkName();
        cout << "Creating datastore: " << name << endl;
        if( !db->createStore( name ) ){
            cerr << "ERROR: Can't create DataStore '" << name << "'\n";
            return false;
        }

        auto startt = getConfigNumber( "starttime", 0 );

        for( auto && c: m_clients ){
            c->setDataDir( m_datadir );
            c->setDataStore( name, host, port );
            c->setStartTime( startt );

            for( auto && nr: m_newRaster ){
                c->createRaster( nr );
            }
        }

        auto && ds = Engine::DataServer::instance();

        for( auto && nr: m_newRaster ){
            ds->createRaster( nr.key, nr.w, nr.h, nr.x0, nr.x1, nr.y0, nr.y1 );
        }

        return true;
    }

    //--------------------------------------------------------------------------
    void Server::createAgents(){
        auto nClients = m_clients.size();
        if( nClients > 0 ){
            for( const auto & p: m_numAgents ){
                // sort, first the clients with less agents
                sort( begin(m_clients), end(m_clients),
                      []( const unique_ptr<Client> & a,
                          const unique_ptr<Client> & b)
                      {
                          return a->numAgents() < b->numAgents();
                      } );

                cout << "Spawning: " << p.second << " of " << p.first << endl;
                decltype(nClients) nPerClient = p.second / nClients;
                decltype(nClients) rem = p.second % nClients;

                // put more agents in the first clients
                decltype(nClients) i;
                for( i = 0 ; i < rem ; ++i ){
                    auto & c = m_clients[i];
                    if( c->createClass( p.first ) ){
                        c->createAgents( p.first, nPerClient+1 );
                    }else{
                        cerr << "WARNING: Class " << p.first << " can't be created" << endl;
                    }
                }

                // then put the rest
                for( i = rem ; i < m_clients.size() ; ++i ){
                    auto & c = m_clients[i];
                    if( c->createClass( p.first ) ){
                        c->createAgents( p.first, nPerClient );
                    }else{
                        cerr << "WARNING: Class " << p.first << " can't be created" << endl;
                    }
                }
            }
        }else{
            cerr << "WARNING: No clients" << endl;
        }
    }

    //--------------------------------------------------------------------------
    int Server::getConfigInt( const string & key, const int d ) const{
        const auto i = m_config.find( key );
        if( i != m_config.end() ){
            if( i->second.getType() != Util::ScriptValue::ValueType::NUMBER ){
                cerr << "WARNING: '" << key << "' is not a number\n";
            }
            return static_cast<int>(i->second.getNumber(d));
        }else{
            return d;
        }
    }

    //--------------------------------------------------------------------------
    double Server::getConfigNumber( const string & key, const double d ) const{
        const auto i = m_config.find( key );
        if( i != m_config.end() ){
            if( i->second.getType() != Util::ScriptValue::ValueType::NUMBER ){
                cerr << "WARNING: '" << key << "' is not a number\n";
            }
            return i->second.getNumber(d);
        }else{
            return d;
        }
    }

    //--------------------------------------------------------------------------
    string Server::getConfigString( const string & key, const string & d ) const{
        const auto i = m_config.find( key );
        if( i != m_config.end() ){
            if( i->second.getType() != Util::ScriptValue::ValueType::STRING ){
                cerr << "WARNING: '" << key << "' is not a string\n";
            }
            return i->second.getString(d);
        }else{
            return d;
        }
    }

    //--------------------------------------------------------------------------
    void Server::run(){
        auto iters = getConfigInt( "iters", 10 );
        auto startt = getConfigNumber( "starttime", 0 );
        auto endt = getConfigNumber( "endtime", 10 );

        double delta = iters > 0 ? (endt - startt) / static_cast<double>(iters) : 0;

        cout << "\nSERVER: Start Simulation\n\n";
        for( int i = 0 ; i < iters ; i++ ){
            cout << "SERVER: iteration " << i+1 << endl;
            for( auto && c: m_clients ){
                c->runAgents( delta );
            }

            waitClients();
        }

        for( auto && c: m_clients ){
            c->end();
        }

        if( m_dataServer ){
            m_dataServer->end();
        }

        cout << "\nSERVER: End Simulation\n\n";
    }

    //--------------------------------------------------------------------------
    void Server::waitClients() const{
        MPI_Barrier( m_comm );
    }
}

//------------------------------------------------------------------------------
