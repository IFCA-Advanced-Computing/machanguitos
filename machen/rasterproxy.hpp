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
/** @file rasterproxy.hpp
    @brief Data::RasterProxy declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef RASTERPROXY_HPP
#define RASTERPROXY_HPP

//------------------------------------------------------------------------------
#include "raster.hpp"

//------------------------------------------------------------------------------
namespace Data {

    //--------------------------------------------------------------------------
    class RasterProxy : public Raster {
    public:
        RasterProxy( const std::string name, int w, int h, double x0, double x1, double y0, double y1 );
        double getValue( int layer, double x, double y ) override;
    };

}//namespace Data

//------------------------------------------------------------------------------
#endif//RASTERPROXY_HPP

//------------------------------------------------------------------------------
