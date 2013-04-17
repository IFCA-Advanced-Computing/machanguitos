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
#include <mpi.h>
#include <cassert>
#include "dataserverremote.hpp"
#include "mpidefs.hpp"
#include "config.h"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;

    //--------------------------------------------------------------------------
    void DataServerRemote::end(){
        int32_t val{0};
        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK, MpiTag::END, MPI_COMM_WORLD);
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::createRaster( const string & key, int w, int h,
                                         double x0, double x1, double y0, double y1 ){
        assert( key.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val{w};
        char * ckey = new char [key.length()+1];
        strcpy( ckey, key.c_str() );

        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        MPI_Send( ckey, key.length(), MPI_CHAR, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        int32_t ival{h};
        MPI_Send( &ival, 1, MPI_INT, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        double dvals[]{x0, x1, y0, y1};
        MPI_Send( dvals, 4, MPI_DOUBLE, DATASERVER_RANK,
                  MpiTagDS::CREATERASTER, MPI_COMM_WORLD );
        delete[] ckey;
    }

    //--------------------------------------------------------------------------
    void DataServerRemote::createRasterProxy( const string & key, int w, int h,
                                              double x0, double x1, double y0, double y1 ){
        cout << "DataServerRemote::createRasterProxy " << key << endl;
        m_rasters[key] = std::make_shared<Data::Raster>();
    }

}//namespace Engine

//------------------------------------------------------------------------------
