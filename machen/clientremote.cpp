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
/** @file clientremote.cpp
    @brief Engine::ClientRemote Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "clientremote.hpp"
#include <cstdint>
#include <cassert>
#include <cstring>
#include "common/mpi.hpp"
#include "common/log.hpp"
#include "config.h"
#include "mpidefs.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    ClientRemote::ClientRemote( int dest ) : m_dest{dest}, m_numAgents{0} {
        // empty
    }

    //--------------------------------------------------------------------------
    ClientRemote::~ClientRemote(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool ClientRemote::createClass( const string & name ){
        assert( name.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val{0};
        char * cname = new char [name.length()+1];
        strcpy( cname, name.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTagCS::CREATECLASS, MPI_COMM_WORLD );
        MPI_Send( cname, name.length(), MPI_CHAR, m_dest, MpiTagCS::CREATECLASS, MPI_COMM_WORLD );
        delete[] cname;
        return true;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::setStartTime( const double time ){
        int32_t val{0};
        double t{time};
        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTagCS::SETSTARTTIME, MPI_COMM_WORLD );
        MPI_Send( &t, 1, MPI_DOUBLE, m_dest, MpiTagCS::SETSTARTTIME, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void ClientRemote::createRaster( const Data::RasterNewData & raster ){
        Util::LOGD( "ClientRemote::createRaster ", raster.key );
        assert( raster.key.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val{raster.w};
        int32_t ivals[]{raster.h,raster.layers};
        double dvals[]{raster.x0, raster.x1, raster.y0, raster.y1, raster.d};
        char * ckey = new char [raster.key.length()+1];
        strcpy( ckey, raster.key.c_str() );

        switch( raster.rasterType ){
        case Data::RasterNewType::RNT_EMPTY:
            MPI_Send( &val, 1, MPI_INT, m_dest, MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD );
            MPI_Send( ckey, raster.key.length(), MPI_CHAR, m_dest,
                      MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD );
            MPI_Send( ivals, 2, MPI_INT, m_dest, MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD );
            MPI_Send( dvals, 5, MPI_DOUBLE, m_dest,
                      MpiTagCS::CREATERASTERCLIENT, MPI_COMM_WORLD );
            break;

        case Data::RasterNewType::RNT_FILE:
            MPI_Send( &val, 1, MPI_INT, m_dest, MpiTagCS::LOADRASTERCLIENT,
                      MPI_COMM_WORLD );
            MPI_Send( ckey, raster.key.length(), MPI_CHAR, m_dest,
                      MpiTagCS::LOADRASTERCLIENT, MPI_COMM_WORLD );

            char * cfile = new char [raster.filename.length()+1];
            strcpy( cfile, raster.filename.c_str() );

            MPI_Send( cfile, raster.filename.length(), MPI_CHAR, m_dest,
                      MpiTagCS::LOADRASTERCLIENT, MPI_COMM_WORLD );
            MPI_Send( dvals, 4, MPI_DOUBLE, m_dest,
                      MpiTagCS::LOADRASTERCLIENT, MPI_COMM_WORLD );

            delete[] cfile;
            break;
        }

        delete[] ckey;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::createAgents( const string & name, int n ){
        assert( name.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val = n;
        char * cname = new char [name.length()+1];
        strcpy( cname, name.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTagCS::CREATEAGENTS, MPI_COMM_WORLD );
        MPI_Send( cname, name.length(), MPI_CHAR, m_dest, MpiTagCS::CREATEAGENTS, MPI_COMM_WORLD );

        m_numAgents += n;
        delete[] cname;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::runAgents( const double delta ){
        int32_t val{0};
        double d{delta};
        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTagCS::RUNAGENTS, MPI_COMM_WORLD );
        MPI_Send( &d, 1, MPI_DOUBLE, m_dest, MpiTagCS::RUNAGENTS, MPI_COMM_WORLD );
    }

}

//------------------------------------------------------------------------------
