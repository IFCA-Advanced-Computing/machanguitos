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
#include "raster.hpp"

//------------------------------------------------------------------------------
class GDALDataset;

//------------------------------------------------------------------------------
namespace Data {
    //--------------------------------------------------------------------------
    class RasterGDAL : public Raster {
    public:
        RasterGDAL( int w, int h, double x0, double x1, double y0, double y1 );
        ~RasterGDAL();

    private:
        GDALDataset * m_data;
        double m_x0;
        double m_x1;
        double m_y0;
        double m_y1;
    };

}//namespace Data


//------------------------------------------------------------------------------
#endif//RASTERGDAL_HPP

//------------------------------------------------------------------------------
