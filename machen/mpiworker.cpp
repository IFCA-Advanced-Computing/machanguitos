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
#include "common/log.hpp"
#include "common/datastore.hpp"
#include "common/mpi.hpp"
#include "mpidefs.hpp"
#include "engine.hpp"
#include "config.h"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace Util;

    //--------------------------------------------------------------------------
    MPIWorker::MPIWorker(){
        MPI_Comm_rank( MPI_COMM_WORLD, &m_rank );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::run(){
        int32_t val{0};
        MPI_Status status;
        m_running = true;

        while( m_running ){
            MPI_Recv( &val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
            if( status.MPI_ERROR != MPI_SUCCESS ){
                LOGE( "Received on ", m_rank );
                Engine::abort();
            }

            LOGD( "Message[", status.MPI_TAG, "] on ", m_rank, " from ", status.MPI_SOURCE );

            bool is_checked = doCommonTags( status.MPI_TAG, val );
            if( not is_checked ){
                is_checked = doTags( status.MPI_TAG, val );
            }

            if( not is_checked ){
                LOGE( "Not-implemented message[", status.MPI_TAG, "] on ", m_rank,
                      " from ", status.MPI_SOURCE );
            }
        }

    }

    //--------------------------------------------------------------------------
    bool MPIWorker::doCommonTags( int tag, int32_t val ){
        switch( tag ){
        case MpiTag::SETDATASTORE:
            runSetDataStore( val );
            break;

        case MpiTag::SETDATAPATH:
            runSetDataPath();
            break;

        case MpiTag::SETLOGLEVEL:
            setLogLevel( val );
            break;

        case MpiTag::END:
            m_running = false;
            break;

        default:
            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runSetDataPath(){
        char cname[MAX_PATH_NAME+1];
        MPI_Status status;
        MPI_Recv( &cname, MAX_PATH_NAME, MPI_CHAR, 0, MpiTag::SETDATAPATH,
                  MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        cname[count] = 0;

        setDataDir( cname );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runSetDataStore( const int port ){
        char cname[MAX_DB_NAME+1];
        MPI_Status status;
        MPI_Recv( &cname, MAX_DB_NAME, MPI_CHAR, 0, MpiTag::SETDATASTORE,
                  MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        cname[count] = 0;

        char chost[MAX_HOST_NAME+1];
        MPI_Recv( &chost, MAX_HOST_NAME, MPI_CHAR, 0, MpiTag::SETDATASTORE,
                  MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        MPI_Get_count( &status, MPI_CHAR, &count );
        chost[count] = 0;

        auto db = IO::DataStore::instance();

        db->setDataStoreName( cname );
        db->setDataStoreHost( chost );
        db->setDataStorePort( port );
    }

}//namespace Engine

//------------------------------------------------------------------------------
