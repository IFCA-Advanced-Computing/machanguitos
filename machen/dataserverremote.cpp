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
/** @file dataserverremote.cpp
    @brief DataServerRemote Definitions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include <cassert>
#include "common/mpi.hpp"
#include "dataserverremote.hpp"
#include "mpidefs.hpp"
#include "common/log.hpp"
#include "config.h"
#include "rasterproxy.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;

    //--------------------------------------------------------------------------
    void DataServerRemote::createRaster( const string & key, int l, int w, int h,
                                         double x0, double x1, double y0, double y1,
                                         double d, bool isf )
    {
        Util::LOGD( "DataServerRemote::createRaster ", key );
        assert( key.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val{w};
        char * ckey = new char [key.length()+1];
        strcpy( ckey, key.c_str() );

        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        MPI_Send( ckey, key.length(), MPI_CHAR, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        int32_t ivals[]{h, l, isf};
        MPI_Send( ivals, 3, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        double dvals[]{x0, x1, y0, y1, d};
        MPI_Send( dvals, 5, MPI_DOUBLE, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        delete[] ckey;

        m_rasters[key] = std::make_shared<Data::RasterProxy>( key, l, w, h, x0, x1,
                                                              y0, y1, d, isf );
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::loadRaster( const std::string & key,
                                       const std::string & filename,
                                       double x0, double x1,
                                       double y0, double y1 )
    {
        Util::LOGD( "DataServerRemote::loadRaster ", key, " ", filename );
        assert( key.length() <= MAX_CLASS_NAME && "name too long" );
        assert( filename.length() <= MAX_PATH_NAME && "filename too long" );

        int32_t val;
        char * ckey = new char [key.length()+1];
        strcpy( ckey, key.c_str() );
        char * cfilename = new char [filename.length()+1];
        strcpy( cfilename, filename.c_str() );

        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD );
        MPI_Send( ckey, key.length(), MPI_CHAR, DATASERVER_RANK,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD );
        MPI_Send( cfilename, filename.length(), MPI_CHAR, DATASERVER_RANK,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD );
        double dvals[]{x0, x1, y0, y1};
        MPI_Send( dvals, 4, MPI_DOUBLE, DATASERVER_RANK,
                  MpiTagDS::LOADRASTER, MPI_COMM_WORLD );
        delete[] ckey;
        delete[] cfilename;

        m_rasters[key] = std::make_shared<Data::RasterProxy>( key, 1, 1, 1, x0, x1,
                                                              y0, y1, 0, false );
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::createRasterProxy( const string & key, int l, int w, int h,
                                              double x0, double x1, double y0, double y1,
                                              double d, bool isf )
    {
        Util::LOGD( "DataServerRemote::createRasterProxy '", key, "'" );
        m_rasters[key] = std::make_shared<Data::RasterProxy>( key, l, w, h, x0, x1,
                                                              y0, y1, d, isf );
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::createRasterProxy( const string & key,
                                              const string & filename,
                                              double x0, double x1,
                                              double y0, double y1 )
    {
        Util::LOGD( "DataServerRemote::createRasterProxy '", key, "' ",
                    filename );
        m_rasters[key] = std::make_shared<Data::RasterProxy>( key, 1, 1, 1,
                                                              x0, x1, y0, y1,
                                                              0, false );
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::setRasterUpdate( const string & key,
                                            const string & filename )
    {
        Util::LOGD( "DataServerRemote::setRasterUpdate ", key, " ", filename );
        assert( key.length() <= MAX_CLASS_NAME && "name too long" );
        assert( filename.length() <= MAX_PATH_NAME && "filename too long" );
        int32_t val;
        char * ckey = new char [key.length()+1];
        strcpy( ckey, key.c_str() );
        char * cfilename = new char [filename.length()+1];
        strcpy( cfilename, filename.c_str() );

        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::SETRASTERUPDATE, MPI_COMM_WORLD );
        MPI_Send( ckey, key.length(), MPI_CHAR, DATASERVER_RANK,
                  MpiTagDS::SETRASTERUPDATE, MPI_COMM_WORLD );
        MPI_Send( cfilename, filename.length(), MPI_CHAR, DATASERVER_RANK,
                  MpiTagDS::SETRASTERUPDATE, MPI_COMM_WORLD );

        delete[] ckey;
        delete[] cfilename;
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::updateLayers( const double delta ){
        Util::LOGD( "DataServerRemote::updateLayers" );
        int32_t val{0};
        double d{delta};
        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK, MpiTagDS::UPDATELAYERS,
                  MPI_COMM_WORLD );
        MPI_Send( &d, 1, MPI_DOUBLE, DATASERVER_RANK, MpiTagDS::UPDATELAYERS,
                  MPI_COMM_WORLD );

        int ok;

        MPI_Status status;
        MPI_Recv( &ok, 1, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::UPDATELAYERS, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            Util::LOGE( "Received on data server response" );
            Engine::abort();
        }

    }

}//namespace Engine

//------------------------------------------------------------------------------
