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
/** @file rasterproxy.cpp
    @brief Data::RasterProxy definition.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "rasterproxy.hpp"
#include "common/mpi.hpp"
#include <cassert>
#include <memory>
#include <cstring>
#include "common/log.hpp"
#include "config.h"
#include "mpidefs.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
namespace Data {
    //--------------------------------------------------------------------------
    RasterProxy::RasterProxy( const std::string & key, int l, int w, int h, double x0, double x1, double y0, double y1, double d, bool isf )
        : Raster{key, l, w, h, x0, x1, y0, y1, d, isf}
    {
        assert( key.length() <= MAX_CLASS_NAME && "name too long" );
        m_ckeyLength = key.length();
        m_ckey = std::move( std::unique_ptr<char[]>( new char [m_ckeyLength+1] ) );
        strcpy( m_ckey.get(), key.c_str() );
    }

    //--------------------------------------------------------------------------
    double RasterProxy::getValue( int layer, double x, double y ){
        int32_t val{layer};
        MPI_Send( &val, 1, MPI_INT, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD );
        MPI_Send( m_ckey.get(), m_ckeyLength, MPI_CHAR, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD );
        double dvals[]{x, y};
        MPI_Send( dvals, 2, MPI_DOUBLE, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD );

        MPI_Status status;
        double dval;
        MPI_Recv( &dval, 1, MPI_DOUBLE, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::GETRASTERVALUE, MPI_COMM_WORLD, &status );
        if( status.MPI_ERROR != MPI_SUCCESS ){
            Util::LOGE( "Return Received on RasterProxy" );
            Engine::abort();
        }

        return dval;
    }

    //--------------------------------------------------------------------------
    void RasterProxy::setValue( int layer, double x, double y, double v ){
        int32_t val{layer};
        MPI_Send( &val, 1, MPI_INT, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::SETRASTERVALUE, MPI_COMM_WORLD );
        MPI_Send( m_ckey.get(), m_ckeyLength, MPI_CHAR, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::SETRASTERVALUE, MPI_COMM_WORLD );
        double dvals[]{x, y, v};
        MPI_Send( dvals, 3, MPI_DOUBLE, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::SETRASTERVALUE, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void RasterProxy::incrementValue( int layer, double x, double y, double v ){
        int32_t val{layer};
        MPI_Send( &val, 1, MPI_INT, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::INCRASTERVALUE, MPI_COMM_WORLD );
        MPI_Send( m_ckey.get(), m_ckeyLength, MPI_CHAR, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::INCRASTERVALUE, MPI_COMM_WORLD );
        double dvals[]{x, y, v};
        MPI_Send( dvals, 3, MPI_DOUBLE, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::INCRASTERVALUE, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    double RasterProxy::getPixelValue( int /*layer*/, int /*i*/, int /*j*/ ){
        // empty
        return 0;
    }

    //--------------------------------------------------------------------------
    void RasterProxy::setPixelValue( int /*layer*/, int /*i*/, int /*j*/, double /*val*/ ){
        // empty
    }

    //--------------------------------------------------------------------------
    void RasterProxy::loadImage( const std::string & /*filename*/ ){
        // empty
    }

    //--------------------------------------------------------------------------
    void RasterProxy::save( const std::string & filename ){
        Util::LOGD( "RasterProxy::save ", filename );
        assert( filename.length() <= MAX_PATH_NAME && "name too long" );

        int32_t val;
        char * cfilename = new char [filename.length()+1];
        strcpy( cfilename, filename.c_str() );

        MPI_Send( &val, 1, MPI_INT, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::SAVERASTER, MPI_COMM_WORLD );
        MPI_Send( m_ckey.get(), m_ckeyLength, MPI_CHAR, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::SAVERASTER, MPI_COMM_WORLD );
        MPI_Send( cfilename, filename.length(), MPI_CHAR, Engine::DATASERVER_RANK,
                  Engine::MpiTagDS::SAVERASTER, MPI_COMM_WORLD );

        delete[] cfilename;
    }

    //--------------------------------------------------------------------------
    void RasterProxy::setRasterUpdate( const std::string & /*filename*/ ){
        // empty
    }

    //--------------------------------------------------------------------------
    void RasterProxy::update( const double /*delta*/ ){
        // empty
    }

}//namespace Data

//------------------------------------------------------------------------------
