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
/** @file clientlocal.hpp
    @brief Engine::ClientLocal Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef CLIENTLOCAL_HPP
#define CLIENTLOCAL_HPP

//------------------------------------------------------------------------------
#include <vector>
#include <memory>
#include <random>
#include "client.hpp"
#include "agentinstance.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Client instance that runs in local proccess.
        @ingroup Engine
     */
    class ClientLocal final : public Client{
    public:
        /**
           @param id Client Identifier, and mayor number of Agents in this client.
         */
        ClientLocal( const int id );
        virtual ~ClientLocal();

        void setStartTime( const double time ) override;
        void setRandomSeed( const int32_t seed ) override;
        void createRaster( const Data::RasterNewData & raster ) override;
        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
        void runAgents( const double delta ) override;
        int numAgents() const override;

    private:
        /// list of Agents in this Client instance.
        std::vector<std::unique_ptr<Agent::AgentInstance>> m_objects;
        /// random seed generator for lua;
        std::mt19937 m_gen;
        /// simulation start time
        double m_startTime;
        /// actual simulation time
        double m_totalTime;
        /// client ID. Also mayor ID of Agent Instances.
        int m_ID;
        /// next minor id to use in object creation.
        int m_nextID;
    };

    //--------------------------------------------------------------------------
    inline int ClientLocal::numAgents() const{
        return m_objects.size();
    }

    //--------------------------------------------------------------------------
    inline void ClientLocal::setStartTime( const double time ){
        m_startTime = time;
    }

    //--------------------------------------------------------------------------
    inline void ClientLocal::setRandomSeed( const int32_t seed ){
        m_gen.seed(seed);
    }
}

//------------------------------------------------------------------------------
#endif//CLIENTLOCAL_HPP

//------------------------------------------------------------------------------
