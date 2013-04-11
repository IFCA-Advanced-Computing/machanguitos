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
/** @file datalib.cpp
    @brief Data Library functions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "datalib.hpp"
#include <string>
#include <iostream>
#include <lua.hpp>
#include "dataserver.hpp"
#include "common/util.hpp"

//------------------------------------------------------------------------------
namespace Data {
    //--------------------------------------------------------------------------
    /** Define data.createLayer library function.

        This function create a layer in the datastore.

        @code{.lua}
        data.createLayer( "grass",
                  {
                     x0 = 0.0,
                     x1 = 1.0,
                     y0 = 0.0,
                     y1 = 1.0,
                     w = 100,
                     h = 100,
                  } );
        @endcode

        @param L lua_State.
        @ingroup Config
        @retval 0 No return values to Lua.
     */
    int data_createLayer( lua_State *L ){
        std::string key = luaL_checkstring( L , 1 );

        int w{ 10 };
        int h{ 10 };
        double x0{ 0 }, x1{ 1 }, y0{ 0}, y1{ 1 };

        if( lua_istable( L, 2 ) ){            // 0
            lua_getfield( L, 2, "w" );        // 1
            w = luaL_optint( L, -1, w );      // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "h" );        // 1
            h = luaL_optint( L, -1, h );      // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "x0" );       // 1
            x0 = luaL_optnumber( L, -1, x0 ); // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "x1" );       // 1
            x1 = luaL_optnumber( L, -1, x1 ); // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "y0" );       // 1
            y0 = luaL_optnumber( L, -1, y0 ); // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "y1" );       // 1
            y1 = luaL_optnumber( L, -1, y1 ); // 1
            lua_pop( L, 1 );                  // 0
        }

        std::cout << "new raster \n";
        std::cout << "  w = " << w << std::endl;
        std::cout << "  h = " << h << std::endl;
        std::cout << "  x = ( " << x0 << ", " << x1 << " )\n";
        std::cout << "  y = ( " << y0 << ", " << y1 << " )\n";

        return 0;
    }

    //--------------------------------------------------------------------------
    /** List of functions of data lua library for Agent Config file.
        @ingroup Config
     */
    const luaL_Reg datalib[] = {
        {"createLayer",   data_createLayer},
        {nullptr, nullptr}
    };

    //--------------------------------------------------------------------------
    int openlib( lua_State *L ){
        // set functions
        luaL_register( L, "data", datalib );
        // Set constants
        // removes table
        lua_pop( L, 1 );
        return 0;
    }

}//namespace Data

//------------------------------------------------------------------------------
