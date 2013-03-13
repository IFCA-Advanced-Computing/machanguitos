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
//------------------------------------------------------------------------------
#include "mpiworker.h"

#include "config.h"
#include "clientlocal.h"
#include <cassert>

#if defined(HAVE_MPI)
#include <mpi.h>
#include "mpitags.h"
#endif
//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    MPIWorker::MPIWorker( const int r ) : m_rank{r} {
#if defined(HAVE_MPI)
        m_local = new (std::nothrow) Engine::ClientLocal( m_rank );
        if( !m_local ){
            cerr << "ERROR: can't create local agents on worker\n";
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }
#endif
    }

    //--------------------------------------------------------------------------
    MPIWorker::~MPIWorker(){
        if( m_local ){
            delete m_local;
        }
    }

    //--------------------------------------------------------------------------
    void MPIWorker::run(){
#if defined(HAVE_MPI)
        int32_t val{0};
        MPI_Status status;
        bool running = true;

        while( running ){
            MPI_Recv( &val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
            if( status.MPI_ERROR != MPI_SUCCESS ){
                cerr << "ERROR: Received on " << m_rank << endl;
                MPI_Abort( MPI_COMM_WORLD, 0 );
            }

            switch( status.MPI_TAG ){
            case TAG_CREATECLASS:
                runCreateClass();
                break;

            case TAG_CREATEAGENTS:
                runCreateAgents( val );
                break;

            case TAG_RUNAGENTS:
                runAgents();
                break;

            case TAG_SETSTARTTIME:
                runSetStartTime();
                break;

            case TAG_END:
                running = false;
                break;

            default:
                    cerr << "ERROR: not-implemented message[" << status.MPI_TAG
                         << "] on " << m_rank << endl;
            }
        }

#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runSetStartTime(){
#if defined(HAVE_MPI)
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, 0, TAG_SETSTARTTIME, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            cerr << "ERROR: Received on " << m_rank << endl;
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        if( m_local ){
            m_local->setStartTime( val );
        }

#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runCreateClass(){
#if defined(HAVE_MPI)
        char val[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &val, MAX_CLASS_NAME, MPI_CHAR, 0, TAG_CREATECLASS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            cerr << "ERROR: Received on " << m_rank << endl;
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        val[count] = 0;

        if( m_local ){
            if( !m_local->createClass( val ) ){
                cerr << "WARNING: Class '" << val << "' can't be created\n";
            }
        }

#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runCreateAgents( const int num ){
#if defined(HAVE_MPI)
        char val[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &val, MAX_CLASS_NAME, MPI_CHAR, 0, TAG_CREATEAGENTS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            cerr << "ERROR: Received on " << m_rank << endl;
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        val[count] = 0;

        if( m_local ){
            m_local->createAgents( val, num );
        }

#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void MPIWorker::runAgents(){
#if defined(HAVE_MPI)
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, 0, TAG_RUNAGENTS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            cerr << "ERROR: Received on " << m_rank << endl;
            MPI_Abort( MPI_COMM_WORLD, 0 );
        }

        if( m_local ){
            m_local->runAgents( val );
        }

#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

}

//------------------------------------------------------------------------------
