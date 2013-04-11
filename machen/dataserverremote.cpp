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
#include "dataserverremote.hpp"
#include "mpidefs.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;

    //--------------------------------------------------------------------------
    void DataServerRemote::end(){
        int32_t val{0};
        MPI_Send( &val, 1, MPI_INT, DATASERVER_RANK, MpiTag::END, MPI_COMM_WORLD);
    }

    //--------------------------------------------------------------------------
    shared_ptr<Util::Raster> DataServerRemote::getRaster( const string & key ) const{
        return nullptr;
    }

}//namespace Engine

//------------------------------------------------------------------------------
