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
/** @file rastergdal.cpp
    @brief Data::RasterGDAL definitions.
    @author Luis Cabellos
*/

//------------------------------------------------------------------------------
#include "rastergdal.hpp"
#include <cassert>
#include <cfloat>
#include <iostream>
#include <boost/filesystem.hpp>
#include "gdal_priv.h"
#include "common/log.hpp"
#include "common/util.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
namespace Data {
    using namespace boost::filesystem;
    using namespace std;

    //--------------------------------------------------------------------------
    RasterGDAL::RasterGDAL( const string & key, int w, int h, double x0, double x1, double y0, double y1, double d )
        : Raster{key, w, h, x0, x1, y0, y1, d }
    {
        auto driver = GetGDALDriverManager()->GetDriverByName( "MEM" );

        m_data = driver->Create( "", w, h, 1, GDT_Float32,  nullptr );
        if( ! m_data ){
            Util::LOGE( "ERROR Creating data" );
            terminate();
        }

        auto rasterBand = m_data->GetRasterBand( 1 );
        if( rasterBand ){
            float pixel = d;
            for( int j = 0 ; j < h ; ++j ){
                for( int i = 0 ; i < w ; ++i ){
                    rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                          &pixel, 1, 1, GDT_Float32, 0, 0 );
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    RasterGDAL::RasterGDAL( const string & key, const string & filename,
                            double x0, double x1, double y0, double y1 )
        : Raster{key, 1, 1, x0, x1, y0, y1, 0.0 }
    {
        auto dir = Engine::getDataDir();
        path fullpath = path(dir) /= filename;

        if( !is_regular_file(fullpath) ){
            Util::LOGE( "Not file for raster '", fullpath, "'" );
            terminate();
        }

        m_data = (GDALDataset *) GDALOpen( fullpath.c_str(), GA_ReadOnly );
        if( ! m_data ){
            Util::LOGE( "ERROR Loading data" );
            terminate();
        }

        m_w = m_data->GetRasterXSize();
        m_h = m_data->GetRasterYSize();
    }

    //--------------------------------------------------------------------------
    RasterGDAL::~RasterGDAL(){
        GDALClose( m_data );
    }

    //--------------------------------------------------------------------------
    double RasterGDAL::getValue( int layer, double x, double y ){
        auto rasterBand = m_data->GetRasterBand( layer + 1 );
        if( rasterBand ){
            auto pos = getPosition( x, y );
            float pixel;

            rasterBand->RasterIO( GF_Read, get<0>(pos), get<1>(pos), 1, 1,
                                  &pixel, 1, 1, GDT_Float32, 0, 0 );

            return pixel;
        }else{
            Util::LOGE( "Invalid raster layer ", layer );
        }

        return 0;
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::setValue( int layer, double x, double y, double val ){
        auto rasterBand = m_data->GetRasterBand( layer + 1 );
        if( rasterBand ){
            auto pos = getPosition( x, y );
            float pixel = val;

            rasterBand->RasterIO( GF_Write, get<0>(pos), get<1>(pos), 1, 1,
                                  &pixel, 1, 1, GDT_Float32, 0, 0 );
        }else{
            Util::LOGE( "Invalid raster layer ", layer );
        }
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::save( const string & filename ){
        auto type = Util::getGDALDriverName( filename );
        if( type ){
            auto driver = GetGDALDriverManager()->GetDriverByName( (*type).c_str() );
            if( driver ){
                auto dstDS = driver->CreateCopy( filename.c_str(), m_data, false,
                                                 nullptr, nullptr, nullptr );
                if( dstDS ){
                    GDALClose( dstDS );
                }
            }
        }else{
            Util::LOGE( "Can't get filetype for '", filename, "'" );
        }
    }

}//namespace Data

//------------------------------------------------------------------------------
