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
/** @file dataserverlocal.cpp
    @brief Engine::DataServerLocal Definitions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "dataserverlocal.hpp"
#include <cassert>
#include "gdal_priv.h"
#include "common/log.hpp"
#include "rastergdal.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace std;

    //--------------------------------------------------------------------------
    DataServerLocal::DataServerLocal(){
        GDALAllRegister();
        auto driver = GetGDALDriverManager()->GetDriverByName( "MEM" );
        if( ! driver ){
            Util::LOGE(  "GDAL Mem Driver not found" );
            std::terminate();
        }

        auto metadata = driver->GetMetadata();
        if( ! CSLFetchBoolean( metadata, GDAL_DCAP_CREATE, FALSE ) ){
            Util::LOGE( "Driver Mem not supports Create() method." );
            std::terminate();
        }
    }

    //--------------------------------------------------------------------------
    void DataServerLocal::createRaster( const string & key, int w, int h,
                                        double x0, double x1, double y0, double y1, double d ){
        Util::LOGD( "DataServerLocal::createRaster ", key );
        m_rasters[key] = std::make_shared<Data::RasterGDAL>( key, w, h, x0, x1, y0, y1, d );
    }

    //--------------------------------------------------------------------------
    void DataServerLocal::loadRaster( const string & key, const string & filename,
                                      double x0, double x1, double y0, double y1 ){
        assert( false && "implemented" );
    }

    //--------------------------------------------------------------------------
    void DataServerLocal::createRasterProxy( const string & key, int /*w*/, int /*h*/,
                                             double /*x0*/, double /*x1*/,
                                             double /*y0*/, double /*y1*/, double /*d*/ ){
        Util::LOGD( "DataServerLocal::createRasterProxy ", key );
    }

}//namespace Engine

//------------------------------------------------------------------------------
