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
/** @file raster.hpp
    @brief Util::Raster Declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef RASTER_H
#define RASTER_H

//------------------------------------------------------------------------------
#include <string>
#include <tuple>

//------------------------------------------------------------------------------
namespace Data {
    enum class RasterNewType : unsigned char {
        RNT_EMPTY,
            RNT_FILE,
    };

    //--------------------------------------------------------------------------
    /** Data for used when create a New Raster.
        @ingroup Data
     */
    class RasterNewData{
    public:
        explicit RasterNewData( const std::string & key, int layers, int w, int h,
                                double x0, double x1, double y0, double y1,
                                double d, bool isf );
        explicit RasterNewData( const std::string & key, const std::string & file,
                       double x0, double x1, double y0, double y1 );

        /// type of new raster data.
        RasterNewType rasterType;
        /// name of the raster.
        std::string key;
        /// filename of the raster.
        std::string filename;
        /// updatescript
        std::string updatescript;
        /// number of layers
        int layers;
        /// width of the raster.
        int w;
        /// height of the raster.
        int h;
        /// left value of the raster.
        double x0;
        /// right value of the raster.
        double x1;
        /// top value of the raster.
        double y0;
        /// bottom value of the raster.
        double y1;
        /// default value of the raster.
        double d;
        // is float datatype
        bool isFloat;
    };

    //--------------------------------------------------------------------------
    inline RasterNewData::RasterNewData( const std::string & key, int l,
                                         int w, int h,
                                         double x0, double x1, double y0, double y1,
                                         double d, bool isf  )
        : rasterType{RasterNewType::RNT_EMPTY}, key{ key }, layers{l}, w{ w }, h{ h },
        x0{ x0 }, x1{ x1 }, y0{ y0 }, y1{ y1 }, d{ d }, isFloat{ isf }
    {
        // empty
    }

    //--------------------------------------------------------------------------
    inline RasterNewData::RasterNewData( const std::string & key, const std::string & file,
                                         double x0, double x1, double y0, double y1 )
        : rasterType{RasterNewType::RNT_FILE}, key{ key }, filename{ file }, layers{1},
        w{ 1 }, h{ 1 }, x0{ x0 }, x1{ x1 }, y0{ y0 }, y1{ y1 }, d{ 0 }, isFloat{ false }
    {
        // empty
    }

    //--------------------------------------------------------------------------
    /** Raster Layer Data
        @ingroup Data
     */
    class Raster {
    public:
        Raster( const std::string & key, int layers, int w, int h,
                double x0, double x1, double y0, double y1,
                double d, bool isFloat );
        virtual ~Raster();

        bool validPosition( double x, double y );
        std::tuple<int,int> getPosition( double x, double y );

        virtual double getValue( int layer, double x, double y )=0;
        virtual void setValue( int layer, double x, double y, double val )=0;
        virtual void save( const std::string & filename )=0;
        virtual void setRasterUpdate( const std::string & filename )=0;
        virtual void update( const double delta )=0;
        virtual double getPixelValue( int layer, int i, int j )=0;
        virtual void setPixelValue( int layer, int i, int j, double val )=0;
        virtual void loadImage( const std::string & filename )=0;

        std::string key;

    protected:
        /// number of layers.
        int m_layers;
        /// width of the raster.
        int m_w;
        /// height of the raster.
        int m_h;
        /// left value of the raster.
        double m_x0;
        /// right value of the raster.
        double m_x1;
        /// top value of the raster.
        double m_y0;
        /// bottom value of the raster.
        double m_y1;
        double m_hx;
        double m_hy;
        double m_d;
        bool m_isFloat;
    };
}//namespace Data


//------------------------------------------------------------------------------
#endif//RASTER_H

//------------------------------------------------------------------------------
