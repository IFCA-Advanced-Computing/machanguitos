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
/** @file util.cpp
    @brief Util functions definitions.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "util.hpp"

#include <iostream>

#include "lua.hpp"

//------------------------------------------------------------------------------
namespace Util{
    using namespace std;

    //--------------------------------------------------------------------------
    /** Check a Lua return error, and print a message when is failed.
        @param L lua_State.
        @param ret The return value of a lua function
        @retval true if return value is 0
     */
    bool checkLuaReturn( lua_State * const L, const int ret ){
        if( ret != 0 ){
            auto msg = lua_tostring(L, -1);
            if( msg == nullptr ){
                cerr << "Error: (error object is not a string)\n";
            }else{
                cerr << "Error: " << msg << endl;
            }
            return false;
        }
        return true;
    }

    //--------------------------------------------------------------------------
    /** Generates a Lua warning.
        The error message format is given by fmt plus any extra arguments,
        following the same rules of lua_pushfstring. It also adds at the
        beginning of the message the file name and the line number where the
        error occurred, if this information is available.
        It's similar to luaL_error, but in this case, the funtions returns.
        @param L lua_State.
        @param fmt message format
        @param ... parameters of the message
     */
    void luaL_warn( lua_State *L, const char *fmt, ... ){
        va_list argp;
        va_start( argp, fmt );
        luaL_where( L, 1 );
        lua_pushvfstring( L, fmt, argp );
        va_end( argp );
        lua_concat( L, 2 );
        printf("Warning: %s\n", lua_tostring( L,-1 ) );
        lua_pop( L, 1 );
    }
}

//------------------------------------------------------------------------------
