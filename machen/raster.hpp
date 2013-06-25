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
    //--------------------------------------------------------------------------
    /** Data for used when create a New Raster.
        @ingroup Data
     */
    class RasterNewData{
    public:
        RasterNewData( const std::string & key, int w, int h,
                       double x0, double x1, double y0, double y1, double d );

        /// name of the raster.
        std::string key;
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
    };

    //--------------------------------------------------------------------------
    inline RasterNewData::RasterNewData( const std::string & key, int w, int h,
                                         double x0, double x1, double y0, double y1, double d )
        : key{ key }, w{ w }, h{ h }, x0{ x0 }, x1{ x1 }, y0{ y0 }, y1{ y1 }, d{ d }
    {
        // empty
    }

    //--------------------------------------------------------------------------
    /** Raster Layer Data
        @ingroup Data
     */
    class Raster {
    public:
        Raster( const std::string & key, int w, int h,
                double x0, double x1, double y0, double y1, double d );
        virtual ~Raster();

        bool validPosition( double x, double y );
        std::tuple<int,int> getPosition( double x, double y );

        virtual double getValue( int layer, double x, double y )=0;
        virtual void setValue( int layer, double x, double y, double val )=0;
        virtual bool updateValue( int layer, double x, double y, double old, double val )=0;
        virtual void save( const std::string & filename )=0;

        std::string key;

    protected:
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
    };
}//namespace Data


//------------------------------------------------------------------------------
#endif//RASTER_H

//------------------------------------------------------------------------------
