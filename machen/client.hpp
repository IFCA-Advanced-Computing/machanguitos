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
/** @file client.hpp
    @brief Engine::Client Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef CLIENT_HPP
#define CLIENT_HPP

//------------------------------------------------------------------------------
#include <string>
#include "raster.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Client Interface.

        A Client is the class that should maintain the Agents and execute its
        code.

        @ingroup Engine
     */
    class Client {
    public:
        Client();
        virtual ~Client();

        /** Set the simulation start time.
            @param time start time in seconds.
         */
        virtual void setStartTime( const double time )=0;
        /** Set the simulation random seed.
            @param seed initial seed for the client.
         */
        virtual void setRandomSeed( const int32_t seed )=0;
        virtual void createRaster( const Data::RasterNewData & raster )=0;

        /** Create a AgentClass and save it.
            @param name name of the AgentClass.
            @retval true if it can create the AgentClass instance.
         */
        virtual bool createClass( const std::string & name )=0;
        /** Create Agents of an AgentClass instance.
            @param name name of the AgentClass.
            @param n number of agents to create.
         */
        virtual void createAgents( const std::string & name, int n )=0;
        /** Run a tick of the 'update' function for the Agents in this Client
            instance.
            @param delta time increment in seconds between updates.
         */
        virtual void runAgents( const double delta )=0;
        /// Returns the number of agents in this Client instance.
        virtual int numAgents() const =0;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENT_HPP

//------------------------------------------------------------------------------
