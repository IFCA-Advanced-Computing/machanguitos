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
/** @file mpiclientserver.hpp
    @brief Engine::MPIClientServer Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef MPICLIENTSERVER_HPP
#define MPICLIENTSERVER_HPP

//------------------------------------------------------------------------------
#include <memory>
#include "mpiworker.hpp"
#include "clientlocal.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Define a MPI Worker main loop to run as a Client in a MPI program.

        @ingroup Engine
     */
    class MPIClientServer final : public MPIWorker {
    public:
        /// Construct a MPI Client Server
        MPIClientServer();

    private:
        bool doTags( int tag, int src, int32_t val ) override;
        /** Execute a Set Start Time command.
            @param src caller id.
        */
        void runSetStartTime( int src );
        /** Execute a Create Class command.
            @param src caller id.
        */
        void runCreateClass( int src );
        /** Execute a Create Agents command.
            @param src caller id.
            @param num number of agents to create.
         */
        void runCreateAgents( int src, const int num );
        /** Execute a Create Raster command.
            @param src caller id.
            @param w width of the raster layer.
         */
        void runCreateRasterClient( int src, const int w );
        /** Execute a Load Raster command.
            @param src caller id.
        */
        void runLoadRasterClient( int src );
        /** Execute a runAgents command.
            @param src caller id.
        */
        void runAgents( int src );

        /// Client instance that runs the real code.
        std::unique_ptr<ClientLocal> m_local;
    };
}

//------------------------------------------------------------------------------
#endif//MPICLIENTSERVER_HPP

//------------------------------------------------------------------------------
