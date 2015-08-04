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
#include <lua.hpp>
#include "server.hpp"
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
                     default = 250,
                  } );
        @endcode

        It create a layer with 1 sublayers. But it is possible to specify more
        sublayer with the parameter `layers`, like:

        @code{.lua}
        data.createLayer( "grass",
                  {
                     x0 = 0.0,
                     x1 = 1.0,
                     y0 = 0.0,
                     y1 = 1.0,
                     w = 100,
                     h = 100,
                     default = 250,
                     layers = 3,
                  } );
        @endcode

        The layers are created with Byte type (range [0..255]) but it is
        possible to ask to use Float number (32 bits Float) in the layers using
        the parameter `isFloat`, like:

        @code{.lua}
        data.createLayer( "grass",
                  {
                     x0 = 0.0,
                     x1 = 1.0,
                     y0 = 0.0,
                     y1 = 1.0,
                     w = 100,
                     h = 100,
                     default = 250,
                     isFloat = true,
                  } );
        @endcode

        Float raster layers will be defaulting to byte when saved on formats
        that don't allow Float types (like png, or jpeg). You can use TIFF files
        to save them with Float numbers.

        @param L lua_State.
        @ingroup Config
        @retval 0 No return values to Lua.
     */
    int data_createLayer( lua_State *L ){
        std::string key = luaL_checkstring( L , 1 );

        int w{ 10 };
        int h{ 10 };
        int l{ 1 };
        double x0{ 0 }, x1{ 1 }, y0{ 0 }, y1{ 1 }, def{0};

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

            lua_getfield( L, 2, "default" );  // 1
            def = luaL_optnumber( L, -1, def );// 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "layers" );   // 1
            l = luaL_optnumber( L, -1, l );   // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 2, "isFloat" );    // 1
            bool isFloat = lua_toboolean( L, -1 ); // 1
            lua_pop( L, 1 );                  // 0
        }

        auto engine = Engine::Server::instance();
        engine->createRaster( key, l, w, h, x0, x1, y0, y1, def );

        return 0;
    }

    //--------------------------------------------------------------------------
    /** Define data.loadLayer library function.

        This function create a layer in the datastore and load its contents from
        file.

        @code{.lua}
        data.loadLayer( "area", "area.png",
                  {
                     x0 = 0.0,
                     x1 = 1.0,
                     y0 = 0.0,
                     y1 = 1.0,
                  } );
        @endcode

        @param L lua_State.
        @ingroup Config
        @retval 0 No return values to Lua.
     */
    int data_loadLayer( lua_State *L ){
        std::string key = luaL_checkstring( L , 1 );
        std::string file = luaL_checkstring( L , 2 );

        double x0{ 0 }, x1{ 1 }, y0{ 0 }, y1{ 1 };

        if( lua_istable( L, 3 ) ){            // 0
            lua_getfield( L, 3, "x0" );       // 1
            x0 = luaL_optnumber( L, -1, x0 ); // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 3, "x1" );       // 1
            x1 = luaL_optnumber( L, -1, x1 ); // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 3, "y0" );       // 1
            y0 = luaL_optnumber( L, -1, y0 ); // 1
            lua_pop( L, 1 );                  // 0

            lua_getfield( L, 3, "y1" );       // 1
            y1 = luaL_optnumber( L, -1, y1 ); // 1
            lua_pop( L, 1 );                  // 0
        }

        auto engine = Engine::Server::instance();
        engine->loadRaster( key, file, x0, x1, y0, y1 );

        return 0;
    }

    //--------------------------------------------------------------------------
    /** Define data.setLayerUpdate library function.

        This function sets a script to update the layer during the simulation.

        @code{.lua}
        data.setLayerUpdate( "area", "areascript" );
        @endcode

        @param L lua_State.
        @ingroup Config
        @retval 0 No return values to Lua.
     */
    int data_setLayerUpdate( lua_State *L ){
        std::string key = luaL_checkstring( L , 1 );
        std::string file = luaL_checkstring( L , 2 );

        auto engine = Engine::Server::instance();
        engine->setRasterLayerUpdate( key, file );

        return 0;
    }

    //--------------------------------------------------------------------------
    /** List of functions of data lua library for Agent Config file.
        @ingroup Config
     */
    const luaL_Reg datalib[] = {
        {"createLayer",   data_createLayer},
        {"loadLayer",   data_loadLayer},
        {"setLayerUpdate", data_setLayerUpdate},
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
