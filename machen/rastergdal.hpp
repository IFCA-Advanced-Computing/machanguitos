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
/** @file rastergdal.hpp
    @brief Data::RastarGDAL declarations.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef RASTERGDAL_HPP
#define RASTERGDAL_HPP

//------------------------------------------------------------------------------
#include "lua.hpp"
#include "raster.hpp"

//------------------------------------------------------------------------------
class GDALDataset;

//------------------------------------------------------------------------------
namespace Data {
    //--------------------------------------------------------------------------
    /** Raster Table Name for Lua files.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_RASTER_NAME = "Raster";

    //--------------------------------------------------------------------------
    /** private name of RasterGDAL objects in Lua tables.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_RASTER_OBJ = "__ro";

    //--------------------------------------------------------------------------
    class RasterGDAL final : public Raster {
    public:
        RasterGDAL( const std::string & key, int l, int w, int h,
                    double x0, double x1, double y0, double y1, double d, bool isFloat );
        RasterGDAL( const std::string & key, const std::string & filename,
                    double x0, double x1, double y0, double y1 );
        ~RasterGDAL();

        double getValue( int layer, double x, double y ) override;
        void setValue( int layer, double x, double y, double val ) override;
        void save( const std::string & filename ) override;
        void setRasterUpdate( const std::string & filename ) override;
        void update( const double delta ) override;

        double getPixelValue( int layer, int i, int j ) override;
        void setPixelValue( int layer, int i, int j, double val ) override;

        void loadImage( const std::string & filename ) override;

    private:
        GDALDataset * m_data;

        /// Lua State
        lua_State * m_L = nullptr;
    };

}//namespace Data

//------------------------------------------------------------------------------
#endif//RASTERGDAL_HPP

//------------------------------------------------------------------------------
