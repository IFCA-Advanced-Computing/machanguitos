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
/** @file util.hpp
    @brief Util functions.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef UTIL_HPP
#define UTIL_HPP

//------------------------------------------------------------------------------
#include <memory>

//------------------------------------------------------------------------------
struct lua_State;

//------------------------------------------------------------------------------
namespace Util{
    /** Check a lua function return, and show error message if needed.
        @param L Lua State.
        @param err Lua error state.
        @retval True if Lua Return Value is OK.
        @ingrop Util
     */
    bool checkLuaReturn( lua_State * const L, const int err );
    /** Warning Message function to be used inside a C function called from Lua.
        @param fmt format string of warning.
        @param L Lua State.
        @ingrop Util
     */
    void luaL_warn( lua_State *L, const char *fmt, ... );
    /** Obtain the GDAL Driver Name for a file name
        @param filename file name.
        @returns the GDAL Driver Name or null pointer.
        @ingrop Util
     */
    std::unique_ptr<std::string> getGDALDriverName( const std::string & filename );
}

//------------------------------------------------------------------------------
#endif//UTIL_HPP

//------------------------------------------------------------------------------
