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
/** @file server.hpp
    @brief Engine::Server Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef SERVER_HPP
#define SERVER_HPP

//------------------------------------------------------------------------------
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <forward_list>
#include "lua.hpp"
#include "common/singleton.hpp"
#include "common/scriptvalue.hpp"
#include "client.hpp"
#include "raster.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Singleton Class that controls teh simulation execution.
        @ingroup Engine
     */
    class Server final : public Singleton<Server> {
    public:
        /** Add agents number to create them in the future.

            This method don't create the agent rigth now. It only adds to an
            internal list. It's possible to execute several times with the same
            name. E.g:

            \code
            Server::instance()->addAgents( "cow", 10 );
            Server::instance()->addAgents( "cow", 10 );
            \endcode

            Has the same result of:

            \code
            Server::instance()->addAgents( "cow", 20 );
            \endcode

            @param name of the AgentClass.
            @param n number of Agents to create.
         */
        void addAgents( const std::string & name, const unsigned n );
        /** Configure clients and world.
            @param filename name of the server script to load.
        */
        bool initialize( const std::string & filename );
        /// create the agents for the simulation.
        void createAgents();
        void createRaster( const std::string & key, int w, int h,
                           double x0, double x1, double y0, double y1 );
        /** Insert a config parameter.
            @param key name of the parameter.
            @param val value of the parameter.
         */
        void insertConfig( const std::string & key, Util::ScriptValue && val );
        /// run loop of the simulation.
        void run();

    private:
        /// Create the expectect number of clients.
        void createClients();
        /** Returns the Integer value of a config parameter.
            @param key name of the parameter.
            @param d default value.
            @returns the value, or default if this variable is not integer type.
         */
        int getConfigInt( const std::string & key, const int d ) const;
        /** Returns the numeric value of a config parameter.
            @param key name of the parameter.
            @param d default value.
            @returns the value, or default if this variable is not numeric type.
         */
        double getConfigNumber( const std::string & key, const double d ) const;
        /** Returns the string value of a config parameter.
            @param key name of the parameter.
            @param d default value.
            @returns the value, or default if this variable is not string type.
         */
        std::string getConfigString( const std::string & key, const std::string & d ) const;

        void initializeScript();
        void startIterationScript( const int n );
        void endIterationScript( const int n );
        void endSimulationScript();

        /// list of number of agents to create of each AgentClass.
        std::map<std::string, unsigned> m_numAgents;
        std::forward_list<Data::RasterNewData> m_newRaster;
        /// list of Clients used during simulation.
        std::vector<std::unique_ptr<Client>> m_clients;
        /// parameters loaded from config file.
        std::map<std::string, Util::ScriptValue> m_config;
        /// Lua State
        lua_State * m_L;
    };

    //--------------------------------------------------------------------------
    inline void Server::insertConfig( const std::string & key,
                                      Util::ScriptValue && val ){
        m_config[key] = std::move(val);
    }
}

//------------------------------------------------------------------------------
#endif//SERVER_HPP

//------------------------------------------------------------------------------
