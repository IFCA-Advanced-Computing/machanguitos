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
/** @file raster.cpp
    @brief Data::Raster definition.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "raster.hpp"
#include <tuple>
#include <cmath>
#include "common/log.hpp"

//------------------------------------------------------------------------------
namespace Data {
    //--------------------------------------------------------------------------
    Raster::Raster( const std::string & k, int w, int h,
                    double x0, double x1, double y0, double y1, double d )
        : key{k}, m_w{w}, m_h{h}, m_x0{x0}, m_x1{x1}, m_y0{y0}, m_y1{y1}, m_d{d}
    {
        if( m_w <= 0 or m_h <= 0 ){
            Util::LOGW( "Invalid Raster dimensions ", w, ", ", h );
            std::terminate();
        }
        m_hx = (m_x1 - m_x0) / m_w;
        m_hy = (m_y1 - m_y0) / m_h;

        if( m_hx == 0 or m_hy == 0 ){
            Util::LOGW( "Invalid Raster dimensions ", w, ", ", h );
            std::terminate();
        }
    }

    //--------------------------------------------------------------------------
    Raster::~Raster(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool Raster::validPosition( double x, double y ){
        if( x < m_x0 or x >= m_x1 ){
            return false;
        }

        return (y >= m_y0 and y < m_y1);
    }

    //--------------------------------------------------------------------------
    std::tuple<int,int> Raster::getPosition( double x, double y ){
        int i = round( (x - m_x0) / m_hx );
        int j = round( (y - m_y0) / m_hy );
        return std::make_tuple( i, j );
    }

}//namespace Data

//------------------------------------------------------------------------------
