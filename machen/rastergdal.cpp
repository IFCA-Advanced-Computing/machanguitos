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
    int rastergdal_getpixel( lua_State *L ){
        auto l = luaL_checknumber( L, -3 );
        auto x = luaL_checknumber( L, -2 );
        auto y = luaL_checknumber( L, -1 );

        lua_getfield( L, -4, SCRIPT_RASTER_OBJ );           // 1
        if( lua_islightuserdata( L, -1 ) ){
            auto raster = static_cast<Data::Raster*>( lua_touserdata( L, -1 ) );
            lua_pop( L, 1 );                                // 0
            if( raster ){
                auto val = raster->getPixelValue( l, x, y );
                lua_pushnumber( L, val );                   // 1
                return 1;
            }else{
                luaL_error( L, "Invalid raster object" );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    int rastergdal_setpixel( lua_State *L ){
        auto l = luaL_checknumber( L, -4 );
        auto x = luaL_checknumber( L, -3 );
        auto y = luaL_checknumber( L, -2 );
        auto v = luaL_checknumber( L, -1 );

        lua_getfield( L, -5, SCRIPT_RASTER_OBJ );           // 1
        if( lua_islightuserdata( L, -1 ) ){
            auto raster = static_cast<Data::Raster*>( lua_touserdata( L, -1 ) );
            lua_pop( L, 1 );                                // 0
            if( raster ){
                raster->setPixelValue( l, x, y, v );
            }else{
                luaL_error( L, "Invalid raster object" );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    int rastergdal_load( lua_State *L ){
        auto filename = luaL_checkstring( L, -1 );

        lua_getfield( L, -2, SCRIPT_RASTER_OBJ );           // 1
        if( lua_islightuserdata( L, -1 ) ){
            auto raster = static_cast<Data::Raster*>( lua_touserdata( L, -1 ) );
            lua_pop( L, 1 );                                // 0
            if( raster ){
                raster->loadImage( filename );
            }else{
                luaL_error( L, "Invalid raster object" );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    RasterGDAL::RasterGDAL( const string & key, int layers, int w, int h, double x0, double x1, double y0, double y1, double d )
        : Raster{key, layers, w, h, x0, x1, y0, y1, d }
    {
        auto driver = GetGDALDriverManager()->GetDriverByName( "MEM" );

        m_data = driver->Create( "", w, h, layers, GDT_Float32, nullptr );
        if( ! m_data ){
            LOGE( "ERROR Creating data" );
            terminate();
        }

        float pixel = d;

        for( auto l = 0 ; i < layers ; ++i ){
            auto rasterBand = m_data->GetRasterBand( l+1 );
            if( rasterBand ){
                for( int j = 0 ; j < h ; ++j ){
                    for( int i = 0 ; i < w ; ++i ){
                        rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                              &pixel, 1, 1, GDT_Float32, 0, 0 );
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    RasterGDAL::RasterGDAL( const string & key, const string & filename,
                            double x0, double x1, double y0, double y1 )
        : Raster{key, 1, 1, 1, x0, x1, y0, y1, 0.0 }
    {
        auto dir = Engine::getDataDir();
        path fullpath = path(dir) /= filename;

        if( !is_regular_file(fullpath) ){
            LOGE( "Not file for raster '", fullpath, "'" );
            terminate();
        }

        m_data = static_cast<GDALDataset*>( GDALOpen( fullpath.c_str(), GA_ReadOnly ) );
        if( ! m_data ){
            LOGE( "ERROR Loading data" );
            terminate();
        }

        m_w = m_data->GetRasterXSize();
        m_h = m_data->GetRasterYSize();

        m_layers = m_data->GetRasterCount();
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
            auto i = get<0>(pos);
            auto j = get<1>(pos);
            auto isize = rasterBand->GetXSize();
            auto jsize = rasterBand->GetYSize();
            if( i < 0 or i >= isize or j < 0 or j >= jsize ){
                LOGD( "Out-of-range position get( ", x, ", ", y, " )" );
                return 0;
            }

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
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_byte, 1, 1, datatype, 0, 0 );
                ret = pixel_byte;
                break;

            case GDT_UInt16:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_uint16, 1, 1, datatype, 0, 0 );
                ret = pixel_uint16;
                break;

            case GDT_Int16:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_int16, 1, 1, datatype, 0, 0 );
                ret = pixel_int16;
                break;

            case GDT_UInt32:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_uint32, 1, 1, datatype, 0, 0 );
                ret = pixel_uint32;
                break;

            case GDT_Int32:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_int32, 1, 1, datatype, 0, 0 );
                ret = pixel_int32;
                break;

            case GDT_Float32:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
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
            auto i = get<0>(pos);
            auto j = get<1>(pos);
            auto isize = rasterBand->GetXSize();
            auto jsize = rasterBand->GetYSize();
            if( i < 0 or i >= isize or j < 0 or j >= jsize ){
                LOGD( "Out-of-range position set( ", x, ", ", y, " )" );
                return;
            }

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
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_byte, 1, 1, datatype, 0, 0 );
                break;

            case GDT_UInt16:
                pixel_uint16 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_uint16, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Int16:
                pixel_int16 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_int16, 1, 1, datatype, 0, 0 );
                break;

            case GDT_UInt32:
                pixel_uint32 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_uint32, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Int32:
                pixel_int32 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_int32, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Float32:
                pixel_float32 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
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
    double RasterGDAL::getPixelValue( int layer, int i, int j ){
        auto rasterBand = m_data->GetRasterBand( layer + 1 );
        if( rasterBand ){
            auto isize = rasterBand->GetXSize();
            auto jsize = rasterBand->GetYSize();
            if( i < 0 or i >= isize or j < 0 or j >= jsize ){
                LOGD( "Out-of-range position get( ", i, ", ", j, " )" );
                return 0;
            }

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
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_byte, 1, 1, datatype, 0, 0 );
                ret = pixel_byte;
                break;

            case GDT_UInt16:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_uint16, 1, 1, datatype, 0, 0 );
                ret = pixel_uint16;
                break;

            case GDT_Int16:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_int16, 1, 1, datatype, 0, 0 );
                ret = pixel_int16;
                break;

            case GDT_UInt32:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_uint32, 1, 1, datatype, 0, 0 );
                ret = pixel_uint32;
                break;

            case GDT_Int32:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
                                      &pixel_int32, 1, 1, datatype, 0, 0 );
                ret = pixel_int32;
                break;

            case GDT_Float32:
                rasterBand->RasterIO( GF_Read, i, j, 1, 1,
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
    void RasterGDAL::setPixelValue( int layer, int i, int j, double val ){
        auto rasterBand = m_data->GetRasterBand( layer + 1 );
        if( rasterBand ){
            auto isize = rasterBand->GetXSize();
            auto jsize = rasterBand->GetYSize();
            if( i < 0 or i >= isize or j < 0 or j >= jsize ){
                LOGD( "Out-of-range position set( ", i, ", ", j, " )" );
                return;
            }

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
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_byte, 1, 1, datatype, 0, 0 );
                break;

            case GDT_UInt16:
                pixel_uint16 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_uint16, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Int16:
                pixel_int16 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_int16, 1, 1, datatype, 0, 0 );
                break;

            case GDT_UInt32:
                pixel_uint32 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_uint32, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Int32:
                pixel_int32 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
                                      &pixel_int32, 1, 1, datatype, 0, 0 );
                break;

            case GDT_Float32:
                pixel_float32 = val;
                rasterBand->RasterIO( GF_Write, i, j, 1, 1,
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
    void RasterGDAL::loadImage( const std::string & filename ){
        auto dir = Engine::getDataDir();
        path fullpath = path(dir) /= filename;

        if( !is_regular_file(fullpath) ){
            LOGE( "Not file for raster '", fullpath, "'" );
            terminate();
        }

        auto dataset = static_cast<GDALDataset*>( GDALOpen( fullpath.c_str(), GA_ReadOnly ) );
        if( ! dataset ){
            LOGE( "ERROR Loading data" );
            terminate();
        }

        if( m_data ){
            GDALClose( m_data );
        }

        m_data = dataset;

        m_w = m_data->GetRasterXSize();
        m_h = m_data->GetRasterYSize();

        m_layers = m_data->GetRasterCount();

        // update lua members
        if( m_L ){
            lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME ); // 1
            lua_pushstring( m_L, "width");                             // 2
            lua_pushnumber( m_L, m_h );                                // 3
            lua_settable( m_L, -3 );                                   // 1
            lua_pushstring( m_L, "height");                            // 2
            lua_pushnumber( m_L, m_w );                                // 3
            lua_settable( m_L, -3 );                                   // 1
            lua_pushstring( m_L, "layers");                            // 2
            lua_pushnumber( m_L, m_layers );                           // 3
            lua_settable( m_L, -3 );                                   // 1
            lua_pop( m_L, 1 );                                         // 0
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
    void RasterGDAL::setRasterUpdate( const std::string & name ){
        LOGD( "RasterGDAL::setRasterUpdate '", name, "'" );
        auto dir = Engine::getDataDir();
        path filename = path(dir) /= (name + ".lua");

        if( !is_regular_file(filename) ){
            LOGE( "Not file for raster '", key, "'" );
            return;
        }

        // Lua Initialization
        m_L = luaL_newstate();
        if( !m_L ){
            LOGE( "Can't create Lua State" );
            return;
        }

        lua_gc( m_L, LUA_GCSTOP, 0 );
        luaL_openlibs( m_L );
        lua_newtable( m_L );
        lua_setfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME );
        lua_gc( m_L, LUA_GCRESTART, 0 );

        // execute update file
        auto ret = luaL_dofile( m_L, filename.c_str() );
        if( !checkLuaReturn( m_L, ret ) ){
            LOGE( "Error executing update file" );
            lua_close( m_L );
            m_L = nullptr;
            return;
        }

        // set class methods for raster
        lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME ); // 1
        lua_pushstring( m_L, SCRIPT_RASTER_OBJ );                  // 2
        lua_pushlightuserdata( m_L, (void*)this );                 // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pushstring( m_L, "getpixel");                          // 2
        lua_pushcfunction( m_L, rastergdal_getpixel );             // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pushstring( m_L, "setpixel");                          // 2
        lua_pushcfunction( m_L, rastergdal_setpixel );             // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pushstring( m_L, "load");                              // 2
        lua_pushcfunction( m_L, rastergdal_load );                 // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pushstring( m_L, "width");                             // 2
        lua_pushnumber( m_L, m_h );                                // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pushstring( m_L, "height");                            // 2
        lua_pushnumber( m_L, m_w );                                // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pushstring( m_L, "layers");                            // 2
        lua_pushnumber( m_L, m_layers );                           // 3
        lua_settable( m_L, -3 );                                   // 1
        lua_pop( m_L, 1 );                                         // 0
    }

    //--------------------------------------------------------------------------
    void RasterGDAL::update( const double delta ){
        LOGD( "RasterGDAL::update(", delta, ")" );
        if( m_L ){
            lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME );    // 1
            lua_getfield( m_L, -1, "update");                             // 2
            if( lua_isfunction( m_L, -1 ) ){
                lua_getfield( m_L, LUA_GLOBALSINDEX, SCRIPT_RASTER_NAME );// 3
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
