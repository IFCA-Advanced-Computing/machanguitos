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
/** @file mpidefs.cpp
    @brief Definitions of MPI utils.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "mpidefs.hpp"

//------------------------------------------------------------------------------
namespace Engine{

    //--------------------------------------------------------------------------
    MPI_Comm createClientsComm(){
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );

        if( nprocs < 2 ){
            return MPI_COMM_WORLD;
        }

        MPI_Group MPI_GROUP_WORLD, clients;
        int datarank = DATASERVER_RANK;

        MPI_Comm_group( MPI_COMM_WORLD, &MPI_GROUP_WORLD );
        MPI_Group_excl( MPI_GROUP_WORLD, 1, &datarank, &clients );

        MPI_Comm comm;
        MPI_Comm_create( MPI_COMM_WORLD, clients, &comm );

        return comm;
    }

}

//------------------------------------------------------------------------------
