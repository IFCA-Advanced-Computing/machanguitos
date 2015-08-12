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
#include "common/mpi.hpp"
#include "mpidefs.hpp"
#include "config.h"
#include "dataserver.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    void runCreateRaster( int src, const int w ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        int32_t ivals[3];
        MPI_Recv( ivals, 3, MPI_INT, src,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        double dval[5];
        MPI_Recv( dval, 5, MPI_DOUBLE, src,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto && ds = Engine::DataServer::instance();
        ds->createRaster( ckey, ivals[1], w, ivals[0], dval[0], dval[1],
                          dval[2], dval[3], dval[4], ivals[2] );
    }

    //--------------------------------------------------------------------------
    void runGetRasterValue( int src, const int layer ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        double dval[2];
        MPI_Recv( dval, 2, MPI_DOUBLE, src,
                  MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto && ds = Engine::DataServer::instance();
        auto && raster = ds->getRaster( ckey );
        double val;
        if( raster ){
            val = raster->getValue( layer, dval[0], dval[1] );
        }else{
            val = 0;
        }

        MPI_Send( &val, 1, MPI_DOUBLE, src,
                  Engine::MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void runSetRasterValue( int src, const int layer ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::SETRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        double dval[3];
        MPI_Recv( dval, 3, MPI_DOUBLE, src,
                  MpiTagDS::SETRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto && ds = Engine::DataServer::instance();
        auto && raster = ds->getRaster( ckey );
        if( raster ){
            raster->setValue( layer, dval[0], dval[1], dval[2] );
        }
    }

    //--------------------------------------------------------------------------
    void runIncRasterValue( int src, const int layer ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::INCRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        double dval[3];
        MPI_Recv( dval, 3, MPI_DOUBLE, src,
                  MpiTagDS::INCRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto && ds = Engine::DataServer::instance();
        auto && raster = ds->getRaster( ckey );
        if( raster ){
            raster->incrementValue( layer, dval[0], dval[1], dval[2] );
        }
    }

    //--------------------------------------------------------------------------
    void runSaveRaster( int src ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::SAVERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        char cfilename[MAX_PATH_NAME+1];
        MPI_Recv( cfilename, MAX_PATH_NAME, MPI_CHAR, src,
                  MpiTagDS::SAVERASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        MPI_Get_count( &status, MPI_CHAR, &count );
        cfilename[count] = 0;

        auto ds = Engine::DataServer::instance();
        auto raster = ds->getRaster( ckey );
        if( raster ){
            raster->save( cfilename );
        }else{
            LOGE( "Invalid Raster '", ckey, "'" );
        }
    }

    //--------------------------------------------------------------------------
    void runLoadRaster( int src ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        char cfilename[MAX_PATH_NAME+1];
        MPI_Recv( cfilename, MAX_PATH_NAME, MPI_CHAR, src,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        MPI_Get_count( &status, MPI_CHAR, &count );
        cfilename[count] = 0;

        double dval[4];
        MPI_Recv( dval, 4, MPI_DOUBLE, src,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto ds = Engine::DataServer::instance();
        ds->loadRaster( ckey, cfilename, dval[0], dval[1], dval[2], dval[3] );
    }

    //--------------------------------------------------------------------------
    void runSetRasterUpdate( int src ){
        char ckey[MAX_CLASS_NAME+1];
        MPI_Status status;
        MPI_Recv( ckey, MAX_CLASS_NAME, MPI_CHAR, src,
                  MpiTagDS::SETRASTERUPDATE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        int count;
        MPI_Get_count( &status, MPI_CHAR, &count );
        ckey[count] = 0;

        char cfilename[MAX_PATH_NAME+1];
        MPI_Recv( cfilename, MAX_PATH_NAME, MPI_CHAR, src,
                  MpiTagDS::SETRASTERUPDATE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        MPI_Get_count( &status, MPI_CHAR, &count );
        cfilename[count] = 0;

        auto ds = Engine::DataServer::instance();
        ds->setRasterUpdate( ckey, cfilename );
    }

    //--------------------------------------------------------------------------
    void runUpdateLayers( int src ){
        double val;
        MPI_Status status;
        MPI_Recv( &val, 1, MPI_DOUBLE, src,
                  MpiTagDS::UPDATELAYERS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            LOGE( "Received on data server" );
            Engine::abort();
        }

        auto ds = Engine::DataServer::instance();
        ds->updateLayers( val );

        int ok = true;

        MPI_Send( &ok, 1, MPI_INT, src, MpiTagDS::UPDATELAYERS, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    MPIDataServer::MPIDataServer() {
        LOGV( "Creating Data Server ", m_rank );
    }

    //--------------------------------------------------------------------------
    bool MPIDataServer::doTags( int tag, int src, int32_t val ){
        switch( tag ){
        case MpiTagDS::CREATERASTER:
            runCreateRaster( src, val );
            break;

        case MpiTagDS::GETRASTERVALUE:
            runGetRasterValue( src, val );
            break;

        case MpiTagDS::SETRASTERVALUE:
            runSetRasterValue( src, val );
            break;

        case MpiTagDS::INCRASTERVALUE:
            runIncRasterValue( src, val );
            break;

        case MpiTagDS::SAVERASTER:
            runSaveRaster( src );
            break;

        case MpiTagDS::LOADRASTER:
            runLoadRaster( src );
            break;

        case MpiTagDS::SETRASTERUPDATE:
            runSetRasterUpdate( src );
            break;

        case MpiTagDS::UPDATELAYERS:
            runUpdateLayers( src );
            break;

        default:
            return false;
        }

        return true;
    }

}//namespace Engine

//------------------------------------------------------------------------------
