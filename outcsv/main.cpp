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
/** @file outcsv/main.cpp
    @brief Main function of machen-outcsv application.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include <cstdlib>
#include <iostream>
#include <getopt.h>

//------------------------------------------------------------------------------
int main( int argc, char * argv[] ){
    constexpr const char * params = "h:p:";
    std::string hoststr{"localhost"};
    std::string portstr{"27017"};
    std::string datastr{""};

    int c;
    while( (c = getopt( argc, argv, params )) != -1 ){
        switch( c ){
        case 'h':
            hoststr = optarg;
            break;

        case 'p':
            portstr = optarg;
            break;

        default:
            std::cerr << "Unknown options\n";
            return EXIT_FAILURE;
        }
    }

    if( argc > optind ){
        datastr = argv[optind];
    }else{
        std::cerr << "There is not namespace name\n";
        return EXIT_FAILURE;
    }

    std::cout << hoststr << ":" << portstr << " " << datastr << std::endl;

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
