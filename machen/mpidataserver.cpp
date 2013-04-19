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
#include "common/log.hpp"
#include "common/datastore.hpp"
#include "mpidefs.hpp"
#include "config.h"
#include "dataserver.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    void runCreateRaster( const int w ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( &ckey, MAX_CLASS_NAME, MPI_CHAR, 0,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        int32_t ival;
        MPI_Recv( &ival, 1, MPI_INT, 0,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        double dval[4];
        MPI_Recv( &dval, 4, MPI_DOUBLE, 0,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto && ds = Engine::DataServer::instance();
        ds->createRaster( ckey, w, ival, dval[0], dval[1], dval[2], dval[3] );
    }

    //--------------------------------------------------------------------------
    MPIDataServer::MPIDataServer() {
        LOGV( "Creating Data Server ", m_rank );
    }

    //--------------------------------------------------------------------------
    bool MPIDataServer::doTags( int tag, int32_t val ){
        switch( tag ){
        case MpiTagDS::CREATERASTER:
            runCreateRaster( val );
            break;

        default:
            return false;
        }

        return true;
    }

}//namespace Engine

//------------------------------------------------------------------------------
