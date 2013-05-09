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
/** @file mpiclientserver.cpp
    @brief Engine::MPIClientServer Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "mpiclientserver.hpp"
#include "config.h"
#include "common/mpi.hpp"
#include "mpidefs.hpp"
#include "dataserver.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    MPIClientServer::MPIClientServer() {
        m_local = unique_ptr<ClientLocal>( new (nothrow) ClientLocal( m_rank ) );
        if( !m_local ){
            LOGE( "Can't create local agents on worker" );
            Engine::abort();
        }
        LOGV( "Creating worker: ", m_rank );
    }

    //--------------------------------------------------------------------------
    bool MPIClientServer::doTags( int tag, int32_t val ){
        switch( tag ){
        case MpiTagCS::CREATECLASS:
            runCreateClass();
            break;

        case MpiTagCS::CREATEAGENTS:
            runCreateAgents( val );
            break;

        case MpiTagCS::RUNAGENTS:
            runAgents();
            break;

        case MpiTagCS::SETSTARTTIME:
            runSetStartTime();
            break;

        case MpiTagCS::CREATERASTERCLIENT:
            runCreateRasterClient( val );
            break;

        default:
            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    void MPIClientServer::runSetStartTime(){
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, 0, MpiTagCS::SETSTARTTIME, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        m_local->setStartTime( val );
    }

    //--------------------------------------------------------------------------
    void MPIClientServer::runCreateClass(){
        char val[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &val, MAX_CLASS_NAME, MPI_CHAR, 0, MpiTagCS::CREATECLASS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        val[count] = 0;

        if( !m_local->createClass( val ) ){
            LOGW( "Class '", val, "' can't be created" );
        }
    }

    //--------------------------------------------------------------------------
    void MPIClientServer::runCreateAgents( const int num ){
        char val[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &val, MAX_CLASS_NAME, MPI_CHAR, 0, MpiTagCS::CREATEAGENTS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        val[count] = 0;

        m_local->createAgents( val, num );
    }

    //--------------------------------------------------------------------------
    void MPIClientServer::runAgents(){
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, 0, MpiTagCS::RUNAGENTS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        m_local->runAgents( val );

        clientsBarrier();
    }

    //--------------------------------------------------------------------------
    void MPIClientServer::runCreateRasterClient( const int w ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &ckey, MAX_CLASS_NAME, MPI_CHAR, 0,
                  MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        int32_t ival;
        MPI_Recv( &ival, 1, MPI_INT, 0,
                  MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        double dval[4];
        MPI_Recv( &dval, 4, MPI_DOUBLE, 0,
                  MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on ", m_rank );
            Engine::abort();
        }

        auto && ds = Engine::DataServer::instance();
        ds->createRasterProxy( ckey, w, ival, dval[0], dval[1], dval[2], dval[3] );
    }
}

//------------------------------------------------------------------------------
