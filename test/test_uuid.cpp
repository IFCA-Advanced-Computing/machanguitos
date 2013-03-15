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
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

//------------------------------------------------------------------------------
int main( int argc, char * argv[] ){
    boost::uuids::random_generator gen;
    boost::uuids::uuid u = gen();

    std::ostringstream strval;
    for( auto i: u ){
        strval.fill('0');
        strval.width(2);
        strval << std::uppercase << std::hex << static_cast<unsigned>(i);
    }
    strval << std::ends;

    std::cout << strval.str() << std::endl;

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
