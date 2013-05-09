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
#include <cassert>
#include "common/mpi.hpp"
#include "config.h"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

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
    void remoteSetDataDir( const string & filename ){
        assert( filename.length() <= MAX_PATH_NAME && "filename too long" );
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
        int32_t val{0};
        char * cname = new char [filename.length()+1];
        strcpy( cname, filename.c_str() );

        for( auto i = 0 ; i < nprocs ; ++i ){
            if( i != SERVER_RANK ){
                MPI_Send( &val, 1, MPI_INT, i,
                          MpiTag::SETDATAPATH, MPI_COMM_WORLD );
                MPI_Send( cname, filename.length(), MPI_CHAR, i,
                          MpiTag::SETDATAPATH, MPI_COMM_WORLD );
            }
        }

        delete[] cname;
    }

    //--------------------------------------------------------------------------
    void remoteSetDataStore( const string & name, const string & host,
                             const uint16_t port )
    {
        assert( name.length() <= MAX_DB_NAME && "name too long" );
        assert( name.length() <= MAX_HOST_NAME && "hostname too long" );
        int nprocs;
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
        int32_t val{port};
        char * cname = new char [name.length()+1];
        strcpy( cname, name.c_str() );
        char * chost = new char [host.length()+1];
        strcpy( chost, host.c_str() );

        for( auto i = 0 ; i < nprocs ; ++i ){
            if( i != SERVER_RANK ){
                MPI_Send( &val, 1, MPI_INT, i, MpiTag::SETDATASTORE, MPI_COMM_WORLD );
                MPI_Send( cname, name.length(), MPI_CHAR, i,
                          MpiTag::SETDATASTORE, MPI_COMM_WORLD );
                MPI_Send( chost, host.length(), MPI_CHAR, i,
                          MpiTag::SETDATASTORE, MPI_COMM_WORLD );
            }
        }

        delete[] cname;
        delete[] chost;
    }

}

//------------------------------------------------------------------------------
