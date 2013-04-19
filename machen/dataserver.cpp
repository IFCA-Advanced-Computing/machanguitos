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
/** @file dataserver.cpp
    @brief DataServer Definition.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "dataserver.hpp"
#include <mpi.h>
#include "dataserverlocal.hpp"
#include "dataserverremote.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    using namespace std;

    //--------------------------------------------------------------------------
    shared_ptr<DataServer> DataServer::s_singleton = nullptr;

    //--------------------------------------------------------------------------
    shared_ptr<DataServer> DataServer::instance(){
        if( ! s_singleton ){
            if( (Engine::isSingleProcess()) or (Engine::isDataServer()) ){
                s_singleton = make_shared<DataServerLocal>();
            }else{
                s_singleton = make_shared<DataServerRemote>();
            }
        }

        return s_singleton;
    }

    //--------------------------------------------------------------------------
    shared_ptr<Data::Raster> DataServer::getRaster( const string & key ) const{
        const auto it = m_rasters.find( key );
        if( it != m_rasters.cend() ){
            return it->second;
        }
        return nullptr;
    }

    //--------------------------------------------------------------------------

}//namespace Engine

//------------------------------------------------------------------------------
