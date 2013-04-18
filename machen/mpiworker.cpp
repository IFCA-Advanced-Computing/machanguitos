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
    @brief Engine::MPIWorker Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "mpiworker.hpp"
#include <mpi.h>
#include "common/log.hpp"
#include "config.h"
#include "clientlocal.hpp"
#include "mpidefs.hpp"
#include "dataserver.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    MPIWorker::MPIWorker( const int r ) : m_rank{r} {
        m_local = unique_ptr<ClientLocal>( new (nothrow) ClientLocal( m_rank ) );
        if( !m_local ){
            LOGE( "Can't create local agents on worker" );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }
        LOGV( "Creating worker: ", m_rank );
        m_comm = createClientsComm();
    }

    //--------------------------------------------------------------------------
    void MPIWorker::run(){
        int32_t val{0};
        MPI_Status status;
        bool running = true;

        while( running ){
            MPI_Recv( &val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
            if( status.MPI_ERROR != MPI_SUCCESS ){
                LOGE( "Received on ", m_rank );
                MPI_Abort( MPI_COMM_WORLD, 0 );
            }

            switch( status.MPI_TAG ){
            case MpiTag::CREATECLASS:
                runCreateClass();
                break;

            case MpiTag::CREATEAGENTS:
                runCreateAgents( val );
                break;

            case MpiTag::RUNAGENTS:
                runAgents();
                break;

            case MpiTag::SETSTARTTIME:
                runSetStartTime();
                break;

            case MpiTag::SETDATASTORE:
                runSetDataStore( val );
                break;

            case MpiTag::SETDATAPATH:
                runSetDataPath();
                break;

            case MpiTag::CREATERASTERCLIENT:
                runCreateRasterClient( val );
                break;

            case MpiTag::END:
                running = false;
                break;

            default:
                LOGE( "Not-implemented message[", status.MPI_TAG, "] on ", m_rank );
            }
        }
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runSetStartTime(){
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, 0, MpiTag::SETSTARTTIME, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        m_local->setStartTime( val );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runSetDataPath(){
        char cname[MAX_PATH_NAME+1];
        MPI_Status status;
        MPI_Recv( &cname, MAX_PATH_NAME, MPI_CHAR, 0, MpiTag::SETDATAPATH, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        cname[count] = 0;

        m_local->setDataDir( cname );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runSetDataStore( const int num ){
        char cname[MAX_DB_NAME+1];
        MPI_Status status;
        MPI_Recv( &cname, MAX_DB_NAME, MPI_CHAR, 0, MpiTag::SETDATASTORE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        cname[count] = 0;

        char chost[MAX_HOST_NAME+1];
        MPI_Recv( &chost, MAX_HOST_NAME, MPI_CHAR, 0, MpiTag::SETDATASTORE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        MPI_Get_count( &status, MPI_CHAR, &count );
        chost[count] = 0;

        m_local->setDataStore( cname, chost, num );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runCreateClass(){
        char val[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &val, MAX_CLASS_NAME, MPI_CHAR, 0, MpiTag::CREATECLASS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        val[count] = 0;

        if( !m_local->createClass( val ) ){
            LOGW( "Class '", val, "' can't be created" );
        }
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runCreateAgents( const int num ){
        char val[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &val, MAX_CLASS_NAME, MPI_CHAR, 0, MpiTag::CREATEAGENTS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        val[count] = 0;

        m_local->createAgents( val, num );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runAgents(){
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, 0, MpiTag::RUNAGENTS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        m_local->runAgents( val );

        MPI_Barrier( m_comm );
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runCreateRasterClient( const int w ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &ckey, MAX_CLASS_NAME, MPI_CHAR, 0,
                  MpiTag::CREATERASTERCLIENT, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        int32_t ival;
        MPI_Recv( &ival, 1, MPI_INT, 0,
                  MpiTag::CREATERASTERCLIENT, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        double dval[4];
        MPI_Recv( &dval, 4, MPI_DOUBLE, 0,
                  MpiTag::CREATERASTERCLIENT, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        auto && ds = Engine::DataServer::instance();
        ds->createRasterProxy( ckey, w, ival, dval[0], dval[1], dval[2], dval[3] );
    }
}

//------------------------------------------------------------------------------
