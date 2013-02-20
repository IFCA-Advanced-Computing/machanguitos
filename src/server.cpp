//------------------------------------------------------------------------------
#include "server.h"

//------------------------------------------------------------------------------
#include <iostream>

#include "client.h"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    void Server::addClient( Client * c ){
        m_clients.push_back( c );
    }

    //--------------------------------------------------------------------------
    void Server::addAgents( const string & name, const unsigned n ){
        m_numAgents[name] = m_numAgents[name] + n;
    }

    //--------------------------------------------------------------------------
    void Server::createAgents(){
        auto nClients = m_clients.size();
        if( nClients > 0 ){
            for( const auto p: m_numAgents ){
                cout << "Creating: " << p.second << " of " << p.first << endl;
                size_t nPerClient = p.second / nClients;
                size_t total = nPerClient * nClients;

                if( total < p.second ){
                    cerr << "WARNING: Creating " << total << " of " << p.second
                         << " " << p.first << " agents" << endl;
                }

                for( auto c: m_clients ){
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
    void Server::run(){
    }
}

//------------------------------------------------------------------------------
