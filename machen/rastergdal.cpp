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
#include <iostream>
#include "gdal_priv.h"

//------------------------------------------------------------------------------
namespace Data {
    //--------------------------------------------------------------------------
    RasterGDAL::RasterGDAL( int w, int h, double x0, double x1, double y0, double y1 )
        : m_x0{ x0 }, m_x1{ x1 }, m_y0{ y0 }, m_y1{ y1 }
    {
        auto driver = GetGDALDriverManager()->GetDriverByName( "MEM" );

        m_data = driver->Create( "", w, h, 1, GDT_Float32,  nullptr );
        if( ! m_data ){
            std::cout << "ERRROR Creating data\n";
            std::terminate();
        }
    }

    //--------------------------------------------------------------------------
    RasterGDAL::~RasterGDAL(){
        GDALClose( m_data );
    }

}//namespace Data

//------------------------------------------------------------------------------
