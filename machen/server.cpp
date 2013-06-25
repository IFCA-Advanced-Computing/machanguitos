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
#include <algorithm>
#include <boost/filesystem.hpp>
#include "common/log.hpp"
#include "common/datastore.hpp"
#include "common/util.hpp"
#include "config.h"
#include "clientlocal.hpp"
#include "clientremote.hpp"
#include "dataserver.hpp"
#include "mpidefs.hpp"
#include "engine.hpp"
#include "configlib.hpp"
#include "datalib.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;
    using namespace boost::filesystem;
    using namespace Util;

    //--------------------------------------------------------------------------
    void Server::createClients(){
        const auto nprocs = Engine::getNumProcesses();
        if( nprocs <= 2 ){
            m_clients.emplace_back( new ClientLocal( 0 ) );
        }

        for( auto i = 2 ; i < nprocs ; ++i ){
            m_clients.emplace_back( new ClientRemote( i ) );
        }
    }

    //--------------------------------------------------------------------------
    void Server::addAgents( const string & name, const unsigned n ){
        if( name.length() > MAX_CLASS_NAME ){
            LOGW( "Class name '", name, "' too long" );
        }else{
            m_numAgents[name] = m_numAgents[name] + n;
        }
    }

    //--------------------------------------------------------------------------
    void Server::createRaster( const string & key, int w, int h,
                       double x0, double x1, double y0, double y1 ){
        if( key.length() > MAX_CLASS_NAME ){
            LOGW( "Raster name '", key, "' too long" );
        }else{
            LOGI( "Creating raster named '", key, "'" );
            m_newRaster.emplace_front( key, w, h, x0, x1, y0, y1 );
        }
    }

    //--------------------------------------------------------------------------
    bool Server::initialize( const string & filename ){
        // Lua Initialization
        m_L = luaL_newstate();
        if( !m_L ){
            LOGE( "Can't create Lua State" );
            return false;
        }

        lua_gc( m_L, LUA_GCSTOP, 0 );
        luaL_openlibs( m_L );
        Config::openlib( m_L );
        Data::openlib( m_L );
        lua_gc( m_L, LUA_GCRESTART, 0 );

        // check file
        if( !exists( filename ) || !is_regular_file( filename ) ){
            LOGE( "There is no config file '", filename, "'" );
            return false;
        }

        // execute config file
        auto ret = luaL_dofile( m_L, filename.c_str() );
        if( not checkLuaReturn( m_L, ret ) ){
            LOGE( "Server Script not loaded" );
            return false;
        }

        initializeScript();
        createClients();

        auto db = IO::DataStore::instance();
        auto host = getConfigString( "dbhost", IO::DataStore::DEFAULT_HOSTNAME );
        auto port = getConfigInt( "dbport", IO::DataStore::DEFAULT_PORT );
        db->setDataStoreHost( host );
        db->setDataStorePort( port );

        auto name = db->mkName();
        LOGI( "Creating datastore: ", name );
        if( !db->createStore( name ) ){
            LOGE( "Can't create DataStore ", name );
            return false;
        }

        auto startt = getConfigNumber( "starttime", 0 );

        auto ll = getLogLevel();
        auto datadir = getDataDir();
        remoteSetLogLevel( ll );
        remoteSetDataDir( datadir );
        remoteSetDataStore( name, host, port );

        for( auto && c: m_clients ){
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

                LOGI( "Spawning: ", p.second, " of '", p.first, "'" );
                decltype(nClients) nPerClient = p.second / nClients;
                decltype(nClients) rem = p.second % nClients;

                // put more agents in the first clients
                decltype(nClients) i;
                for( i = 0 ; i < rem ; ++i ){
                    auto & c = m_clients[i];
                    if( c->createClass( p.first ) ){
                        c->createAgents( p.first, nPerClient+1 );
                    }else{
                        LOGW( "Class '", p.first, "' can't be created" );
                    }
                }

                // then put the rest
                for( i = rem ; i < m_clients.size() ; ++i ){
                    auto & c = m_clients[i];
                    if( c->createClass( p.first ) ){
                        c->createAgents( p.first, nPerClient );
                    }else{
                        LOGW( "Class '", p.first, "' can't be created" );
                    }
                }
            }
        }else{
            LOGW( "No clients" );
        }
    }

    //--------------------------------------------------------------------------
    int Server::getConfigInt( const string & key, const int d ) const{
        const auto i = m_config.find( key );
        if( i != m_config.end() ){
            if( i->second.getType() != ScriptValue::ValueType::NUMBER ){
                LOGW( "Config '", key, "' is not a number" );
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
            if( i->second.getType() != ScriptValue::ValueType::NUMBER ){
                LOGW( "Config '", key, "' is not a number" );
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
            if( i->second.getType() != ScriptValue::ValueType::STRING ){
                LOGW( "Config '", key, "' is not a string" );
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

        LOGI( "SERVER: Start Simulation\n" );
        for( auto i = 0 ; i < iters ; ++i ){
            startIterationScript( i+1 );
            for( auto && c: m_clients ){
                c->runAgents( delta );
            }

            clientsBarrier();
            endIterationScript( i+1 );
        }

        remoteEnd();

        endSimulationScript();
    }

    //--------------------------------------------------------------------------
    void Server::initializeScript(){
        lua_getfield( m_L, LUA_GLOBALSINDEX, "initialize" );        // 1
        if( lua_isfunction( m_L, -1 ) ){
            auto ret = lua_pcall( m_L, 0, 0, 0 );                   // 0
            checkLuaReturn( m_L, ret );
        }else{
            lua_pop( m_L, 1 );                                      // 0
        }
    }
    //--------------------------------------------------------------------------
    void Server::startIterationScript( const int n ){
        lua_getfield( m_L, LUA_GLOBALSINDEX, "startIteration" );    // 1
        if( lua_isfunction( m_L, -1 ) ){
            lua_pushnumber( m_L, n );                               // 2
            auto ret = lua_pcall( m_L, 1, 0, 0 );                   // 0
            checkLuaReturn( m_L, ret );
        }else{
            lua_pop( m_L, 1 );                                      // 0
        }
    }

    //--------------------------------------------------------------------------
    void Server::endIterationScript( const int n ){
        lua_getfield( m_L, LUA_GLOBALSINDEX, "endIteration" );      // 1
        if( lua_isfunction( m_L, -1 ) ){
            lua_pushnumber( m_L, n );                               // 2
            auto ret = lua_pcall( m_L, 1, 0, 0 );                   // 0
            checkLuaReturn( m_L, ret );
        }else{
            lua_pop( m_L, 1 );                                      // 0
        }
    }

    //--------------------------------------------------------------------------
    void Server::endSimulationScript(){
        lua_getfield( m_L, LUA_GLOBALSINDEX, "endSimulation" );     // 1
        if( lua_isfunction( m_L, -1 ) ){
            auto ret = lua_pcall( m_L, 0, 0, 0 );                   // 0
            checkLuaReturn( m_L, ret );
        }else{
            lua_pop( m_L, 1 );                                      // 0
        }
    }
}

//------------------------------------------------------------------------------
