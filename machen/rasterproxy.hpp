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
#include <memory>

//------------------------------------------------------------------------------
namespace Data {

    //--------------------------------------------------------------------------
    class RasterProxy final : public Raster {
    public:
        RasterProxy( const std::string & key, int w, int h, double x0, double x1, double y0, double y1 );
        double getValue( int layer, double x, double y ) override;
        void setValue( int layer, double x, double y, double val ) override;
        bool updateValue( int layer, double x, double y, double old, double val ) override;
        void save( const std::string & filename ) override;
        
    private:
        std::unique_ptr<char[]> m_ckey;
        size_t m_ckeyLength;
    };

}//namespace Data

//------------------------------------------------------------------------------
#endif//RASTERPROXY_HPP

//------------------------------------------------------------------------------
