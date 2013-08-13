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
    using namespace Util;

    //--------------------------------------------------------------------------
    RasterGDAL::RasterGDAL( const string & key, int w, int h, double x0, double x1, double y0, double y1, double d )
        : Raster{key, w, h, x0, x1, y0, y1, d }
    {
        auto driver = GetGDALDriverManager()->GetDriverByName( "MEM" );

        m_data = driver->Create( "", w, h, 1, GDT_Float32,  nullptr );
        if( ! m_data ){
            LOGE( "ERROR Creating data" );
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
            LOGE( "Not file for raster '", fullpath, "'" );
            terminate();
        }

        m_data = (GDALDataset *) GDALOpen( fullpath.c_str(), GA_ReadOnly );
        if( ! m_data ){
            LOGE( "ERROR Loading data" );
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
            auto x = get<0>(pos);
            auto y = get<1>(pos);
            auto datatype = rasterBand->GetRasterDataType();
            double ret = 0;
            uint8_t pixel_byte;
            uint16_t pixel_uint16;
            int16_t pixel_int16;
            uint32_t pixel_uint32;
            int32_t pixel_int32;
            float pixel_float32;

            switch( datatype ){
            case GDT_Byte:
                rasterBand->RasterIO( GF_Read, x, y, 1, 1,
                                      &pixel_byte, 1, 1, datatype, 0, 0 );
                ret = pixel_byte;
                break;

            case GDT_UInt16:
                rasterBand->RasterIO( GF_Read, x, y, 1, 1,
                                      &pixel_uint16, 1, 1, datatype, 0, 0 );
                ret = pixel_uint16;
                break;

            case GDT_Int16:
                rasterBand->RasterIO( GF_Read, x, y, 1, 1,
                                      &pixel_int16, 1, 1, datatype, 0, 0 );
                ret = pixel_int16;
                break;

            case GDT_UInt32:
                rasterBand->RasterIO( GF_Read, x, y, 1, 1,
                                      &pixel_uint32, 1, 1, datatype, 0, 0 );
                ret = pixel_uint32;
                break;

            case GDT_Int32:
                rasterBand->RasterIO( GF_Read, x, y, 1, 1,
                                      &pixel_int32, 1, 1, datatype, 0, 0 );
                ret = pixel_int32;
                break;

            case GDT_Float32:
                rasterBand->RasterIO( GF_Read, x, y, 1, 1,
                                      &pixel_float32, 1, 1, datatype, 0, 0 );
                ret = pixel_float32;
                break;

            default:
                LOGE( "Raster Unknown type" );
                break;

            }

            return ret;
        }else{
            LOGE( "Invalid raster layer ", layer );
        }

        return 0;
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::setValue( int layer, double x, double y, double val ){
        auto rasterBand = m_data->GetRasterBand( layer + 1 );
        if( rasterBand ){
            auto pos = getPosition( x, y );
            auto x = get<0>(pos);
            auto y = get<1>(pos);
            auto datatype = rasterBand->GetRasterDataType();
            uint8_t pixel_byte;
            uint16_t pixel_uint16;
            int16_t pixel_int16;
            uint32_t pixel_uint32;
            int32_t pixel_int32;
            float pixel_float32;

            switch( datatype ){
            case GDT_Byte:
                pixel_byte = val;
                rasterBand->RasterIO( GF_Write, x, y, 1, 1,
                                      &pixel_byte, 1, 1, datatype, 0, 0 );
                break;

            case GDT_UInt16:
                pixel_uint16 = val;
                rasterBand->RasterIO( GF_Write, x, y, 1, 1,
                                      &pixel_uint16, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Int16:
                pixel_int16 = val;
                rasterBand->RasterIO( GF_Write, x, y, 1, 1,
                                      &pixel_int16, 1, 1, datatype, 0, 0 );
                break;

            case GDT_UInt32:
                pixel_uint32 = val;
                rasterBand->RasterIO( GF_Write, x, y, 1, 1,
                                      &pixel_uint32, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Int32:
                pixel_int32 = val;
                rasterBand->RasterIO( GF_Write, x, y, 1, 1,
                                      &pixel_int32, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Float32:
                pixel_float32 = val;
                rasterBand->RasterIO( GF_Write, x, y, 1, 1,
                                      &pixel_float32, 1, 1, datatype, 0, 0 );
                break;

            default:
                LOGE( "Raster Unknown type" );
                break;
            }
        }else{
            LOGE( "Invalid raster layer ", layer );
        }
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::save( const string & filename ){
        auto type = getGDALDriverName( filename );
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
            LOGE( "Can't get filetype for '", filename, "'" );
        }
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::setRasterUpdate( const std::string & filename ){
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::update( const double delta ){
        LOGD( "RasterGDAL::update(", delta, ")" );
        if( m_L ){
            lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME );    // 1
            lua_getfield( m_L, -1, "update");                             // 2
            if( lua_isfunction( m_L, -1 ) ){
                lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME );// 3
                lua_pushlightuserdata( m_L, (void*)this );                // 4
                lua_setglobal( m_L, SCRIPT_GLOBAL_RASTER_OBJ );           // 3
                lua_pushnumber( m_L, delta );                             // 4
                auto ret = lua_pcall( m_L, 2, 0, 0 );                     // 1
                checkLuaReturn( m_L, ret );
                lua_pop( m_L, 1 );                                        // 0
            }else{
                lua_pop( m_L, 2 );                                        // 0
            }
        }
    }

}//namespace Data

//------------------------------------------------------------------------------
