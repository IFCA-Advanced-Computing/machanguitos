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
/** @file mpiworker.cpp
    @brief Engine::MPIWorker definitions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "mpiworker.hpp"
#include <mpi.h>

//------------------------------------------------------------------------------
namespace Engine {
    MPIWorker::MPIWorker(){
        MPI_Comm_rank( MPI_COMM_WORLD, &m_rank );
    }

}//namespace Engine

//------------------------------------------------------------------------------
