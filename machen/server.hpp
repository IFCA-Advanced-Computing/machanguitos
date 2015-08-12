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
        /// Create the agents for the simulation.
        void createAgents();
        /** Create a new Raster object in the simulation.
            @param key name of the Raster object.
            @param l number of layers.
            @param w width in pixels.
            @param h height in pixels.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
            @param d default value of layer.
            @param isFloat true if layer values are Float32, else they will be Bytes.
         */
        void createRaster( const std::string & key, int l, int w, int h,
                           double x0, double x1, double y0, double y1,
                           double d, bool isFloat );
        /** Load a Raster object from a file.
            @param key name of the Raster object.
            @param file filename.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
         */
        void loadRaster( const std::string & key, const std::string & file,
                         double x0, double x1, double y0, double y1 );
        /** Set a Lua update source for a Raster object.

            The Lua file should implement a 'Raster:update' function like:

            @include fireupdate.lua

            @param key name of the Raster object.
            @param file lua filename.
         */
        void setRasterLayerUpdate( const std::string & key,
                                   const std::string & file );
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
        std::string getConfigString( const std::string & key,
                                     const std::string & d ) const;

        /// Call Lua initialize function from the config file.
        void initializeScript();
        /** Call Lua starIteration function from the config file.
            @param n number of current iteration.
         */
        void startIterationScript( const int n );
        /** Call Lua endIteration function from the config file.
            @param n number of current iteration.
         */
        void endIterationScript( const int n );
        /// Call Lua endSimulation function from the config file.
        void endSimulationScript();

        /// list of number of agents to create of each AgentClass.
        std::map<std::string, unsigned> m_numAgents;
        /// list of raster objects to be created.
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
