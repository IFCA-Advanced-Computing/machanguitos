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
        }

        for( auto c: m_clients ){
            c->end();
        }
        cout << "\nSERVER: End Simulation\n\n";
    }
}

//------------------------------------------------------------------------------
