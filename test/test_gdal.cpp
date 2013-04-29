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
//------------------------------------------------------------------------------
#include <cstdlib>
#include <iostream>
#include "gdal_priv.h"
#include "config.h"

//------------------------------------------------------------------------------
int main( int /*argc*/, char */*argv*/[] ){
    std::string filename = std::string(g_datapath) + "/data/test.png";

    GDALAllRegister();

    auto dataset = (GDALDataset *) GDALOpen( filename.c_str(), GA_ReadOnly );
    if( dataset ){
        std::cout << "DataSet opened\n";
        std::cout << "Driver: " << dataset->GetDriver()->GetDescription()
                  << dataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) 
                  << std::endl;
        std::cout << "Size is " << dataset->GetRasterXSize() 
                  << "x" << dataset->GetRasterYSize() 
                  << ", " << dataset->GetRasterCount() 
                  << " raster/s\n";

        const auto pref = dataset->GetProjectionRef();
        if( pref ){
            std::cout << "Projection is '" << pref << "'\n";
        }

        GDALClose( dataset );
    }else{
        std::cout << "DataSet NOT opened\n";
    }

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
