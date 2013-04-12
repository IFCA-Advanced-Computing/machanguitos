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
/** @file mpiworker.hpp
    @brief Engine::MPIWorker Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef MPIWORKER_HPP
#define MPIWORKER_HPP

//------------------------------------------------------------------------------
#include <memory>
#include <mpi.h>
#include "clientlocal.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Define a MPI Process main loop to run as a Worker in a MPI program.

        @ingroup Engine
     */
    class MPIWorker {
    public:
        /** Construct a MPI worker
            @param r MPI rank
        */
        MPIWorker( const int r );

        /// run loop of the MPI worker waiting for Server orders.
        void run();

    private:
        /// Execute a Set Start Time command.
        void runSetStartTime();
        /// Execute a Set DataStore info command.
        void runSetDataStore( const int num );
        /// Execute a Set DataPath info command.
        void runSetDataPath();
        /// Execute a Create Class command.
        void runCreateClass();
        /** Execute a Create Agents command.
            @param num number of agents to create.
         */
        void runCreateAgents( const int num );
        /** Execute a Create Raster command.
            @param w width of the raster layer.
         */
        void runCreateRasterClient( const int w );
        /// Execute a End command.
        void runAgents();

        /// Own MPI rank value.
        int m_rank;
        /// Client instance that runs the real code.
        std::unique_ptr<ClientLocal> m_local;
        /// clients comm group
        MPI_Comm m_comm;
    };
}

//------------------------------------------------------------------------------
#endif//MPIWORKER_HPP

//------------------------------------------------------------------------------
