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
/** @file clientremote.hpp
    @brief Engine::ClientRemote Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef CLIENTREMOTE_HPP
#define CLIENTREMOTE_HPP

//------------------------------------------------------------------------------
#include "client.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    /** Client instance that acts as proxy of a remote MPI worker.
        @ingroup: Engine
     */
    class ClientRemote final : public Client{
    public:
        /** Constructor.
            @param dest MPI rank of remote worker.
         */
        ClientRemote( int dest );
        virtual ~ClientRemote();

        void setStartTime( const double time ) override;
        void setRandomSeed( const int seed ) override;
        void createRaster( const Data::RasterNewData & raster ) override;
        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
        void runAgents( const double delta ) override;
        int numAgents() const override;

    private:
        /// MPI rank of remote worker.
        int m_dest;
        /// number of Agents created in remote.
        int m_numAgents;
    };

    //--------------------------------------------------------------------------
    inline int ClientRemote::numAgents() const{
        return m_numAgents;
    }
}

//------------------------------------------------------------------------------
#endif//CLIENTREMOTE_HPP

//------------------------------------------------------------------------------
