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
#include <mpi.h>
#include <cassert>
#include "config.h"

//------------------------------------------------------------------------------
namespace Engine{

    //--------------------------------------------------------------------------
    void remoteSetLogLevel( Util::LogLevel ll ){
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
        int32_t val{static_cast<int>(ll)};

        for( auto i = 0 ; i < nprocs ; ++i ){
            if( i != SERVER_RANK ){
                MPI_Send( &val, 1, MPI_INT, i, MpiTag::SETLOGLEVEL, MPI_COMM_WORLD);
            }
        }
    }

    //--------------------------------------------------------------------------
    void remoteEnd(){
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
        int32_t val{0};

        for( auto i = 0 ; i < nprocs ; ++i ){
            if( i != SERVER_RANK ){
                MPI_Send( &val, 1, MPI_INT, i, MpiTag::END, MPI_COMM_WORLD);
            }
        }
    }

    //--------------------------------------------------------------------------
    void remoteSetDataDir( const std::string & filename ){
        assert( filename.length() <= MAX_PATH_NAME && "filename too long" );
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
        int32_t val{0};
        char * cname = new char [filename.length()+1];
        strcpy( cname, filename.c_str() );

        for( auto i = 0 ; i < nprocs ; ++i ){
            if( i != SERVER_RANK ){
                MPI_Send( &val, 1, MPI_INT, i,
                          MpiTagCS::SETDATAPATH, MPI_COMM_WORLD );
                MPI_Send( cname, filename.length(), MPI_CHAR, i,
                          MpiTagCS::SETDATAPATH, MPI_COMM_WORLD );
            }
        }
        delete[] cname;
    }

}

//------------------------------------------------------------------------------
