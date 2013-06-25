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
/** @file rasterlib.cpp
    @brief Raster Lua functions definitions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "rasterlib.hpp"
#include <string>
#include <lua.hpp>
#include "common/log.hpp"
#include "dataserver.hpp"

//------------------------------------------------------------------------------
namespace Raster {
    using namespace std;
    using namespace Util;

    constexpr const char * RASTER_OBJ = "__rt";

    //--------------------------------------------------------------------------
    int raster_save( lua_State *L ){
        string name = luaL_checkstring( L, -1 );
        LOGI( "saving raster into '", name, "'" );

        lua_getfield( L, -4, RASTER_OBJ );           // 1
        if( lua_islightuserdata( L, -1 ) ){
            auto raster = (Data::Raster*) lua_topointer( L, -1 );
            lua_pop( L, 1 );                         // 0
            if( raster ){
        //         raster->save( name );
            }else{
                luaL_error( L, "Invalid raster object" );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    /** Define the indexing access of raster table.
        @param L lua_State.
        @ingroup Raster
        @retval 0 No return values to Lua.
     */
    int raster_index( lua_State *L ){
        if( lua_isstring( L, -1 ) ){
            string key = lua_tostring( L, -1 );
            auto ds = Engine::DataServer::instance();

            auto layer = ds->getRaster( key );
            if( layer ){
                lua_newtable( L );                              // 1
                lua_pushstring( L, RASTER_OBJ );                // 2
                lua_pushlightuserdata( L, (void*)layer.get() ); // 3
                lua_rawset( L, -3 );                            // 1
                lua_pushstring( L, "save");                     // 2
                lua_pushcfunction( L, raster_save );            // 3
                lua_settable( L, -3 );                          // 1
                return 1;
            }else{
                luaL_error( L, ("Invalid raster layer '" + key + "'").c_str() );
            }
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    /** List of functions of raster lua library for main file.
        @ingroup Raster
     */
    const luaL_Reg rasterlib[] = {
        {nullptr, nullptr}
    };

    //--------------------------------------------------------------------------
    /** Load raster lib in Lua State
        @param L lua_State.
        @ingroup Raster
        @retval 0 No return values to Lua.
     */
    int openlib( lua_State *L ){
        // set functions
        luaL_register( L, "raster", rasterlib );                // 1
        // set "raster" data
        luaL_newmetatable( L, "raster" );                       // 2
        lua_pushstring( L, "__index");                          // 3
        lua_pushcfunction( L, raster_index );                   // 4
        lua_settable( L, -3 );                                  // 2
        lua_setmetatable( L, -2 );                              // 1
        // Set constants
        // removes table
        lua_pop( L, 1 );                                        // 0
        return 0;
    }

}//namespace Raster

//------------------------------------------------------------------------------
