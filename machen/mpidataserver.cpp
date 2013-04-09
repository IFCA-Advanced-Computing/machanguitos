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
/** @file mpidataserver.cpp
    @brief Engine::MpiDataServer definition.
    @author Luis Cabellos
*/

//------------------------------------------------------------------------------
#include "mpidataserver.hpp"
#include <mpi.h>
#include "mpidefs.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;

    //--------------------------------------------------------------------------
    MPIDataServer::MPIDataServer() {
        /*MPI_Comm comm = */createClientsComm();
        cout << "Creating Data Server\n";
    }

    //--------------------------------------------------------------------------
    void MPIDataServer::run(){
        int32_t val{0};
        MPI_Status status;
        bool running = true;

        while( running ){
            MPI_Recv( &val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
            if( status.MPI_ERROR != MPI_SUCCESS ){
                cerr << "ERROR: Received on data server\n";
                MPI_Abort( MPI_COMM_WORLD, 0 );
            }

            switch( status.MPI_TAG ){
            case MpiTag::END:
                running = false;
                break;

            default:
                    cerr << "ERROR: not-implemented message[" << status.MPI_TAG
                         << "] on data server\n";
            }
        }

    }
    
}//namespace Engine

//------------------------------------------------------------------------------
