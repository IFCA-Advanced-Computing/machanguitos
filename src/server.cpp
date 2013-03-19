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
#include "server.h"

//------------------------------------------------------------------------------
#include <iostream>
#include <algorithm>

#include "config.h"
#include "client.h"
#include "datastore.h"

#if defined(HAVE_MPI)
#include "mpi.h"
#endif

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    void Server::addClient( Client * c ){
        m_clients.push_back( c );
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
    bool Server::initialize(){
        auto db = IO::DataStore::instance();

        auto name = db->mkName();
        cout << "creating datastore: " << name << endl;
        if( !db->createStore( name ) ){
            cerr << "ERROR: Can't create DataStore '" << name << "'\n";
            return false;
        }

        auto startt = getConfigNumber( "starttime", 0 );

        for( auto c: m_clients ){
            c->setStartTime( startt );
        }

        return true;
    }

    //--------------------------------------------------------------------------
    void Server::createAgents(){
        auto nClients = m_clients.size();
        if( nClients > 0 ){
            for( const auto p: m_numAgents ){
                // sort, first the clients with less agents
                sort( m_clients.begin(), m_clients.end(),
                      [](Client * a, Client * b){
                          return a->numAgents() < b->numAgents();
                      } );

                cout << "Creating: " << p.second << " of " << p.first << endl;
                size_t nPerClient = p.second / nClients;
                size_t rem = p.second % nClients;

                // put more agents in the first clients
                size_t i;
                for( i = 0 ; i < rem ; ++i ){
                    auto c = m_clients[i];
                    if( c->createClass( p.first ) ){
                        c->createAgents( p.first, nPerClient+1 );
                    }else{
                        cerr << "WARNING: Class " << p.first << " can't be created" << endl;
                    }
                }

                // then put the rest
                for( i = rem ; i < m_clients.size() ; ++i ){
                    auto c = m_clients[i];
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
    void Server::run(){
        auto iters = getConfigInt( "iters", 10 );
        auto startt = getConfigNumber( "starttime", 0 );
        auto endt = getConfigNumber( "endtime", 10 );

        double delta = iters > 0 ? (endt - startt) / static_cast<double>(iters) : 0;

        cout << "\nSERVER: Start Simulation\n\n";
        for( int i = 0 ; i < iters ; i++ ){
            cout << "SERVER: iteration " << i+1 << endl;
            for( auto c: m_clients ){
                c->runAgents( delta );
            }

            waitClients();
        }

        for( auto c: m_clients ){
            c->end();
        }
        cout << "\nSERVER: End Simulation\n\n";
    }

    //--------------------------------------------------------------------------
    void Server::waitClients() const{
#if defined(HAVE_MPI)
        MPI_Barrier( MPI_COMM_WORLD );
#endif
    }
}

//------------------------------------------------------------------------------
