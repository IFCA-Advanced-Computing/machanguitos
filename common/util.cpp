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
#include <vector>
#include <boost/filesystem.hpp>
#include "lua.hpp"
#include "log.hpp"

//------------------------------------------------------------------------------
namespace Util{
    using namespace std;
    using namespace boost::filesystem;

    //--------------------------------------------------------------------------
    bool checkLuaReturn( lua_State * const L, const int ret ){
        if( ret != 0 ){
            auto msg = lua_tostring(L, -1);
            if( msg == nullptr ){
                Util::LOGE( "Lua error object is not a string" );
            }else{
                Util::LOGE( "Lua: ", msg );
            }
            // remove error message from stack
            lua_pop( L, 1 );
            return false;
        }
        return true;
    }

    //--------------------------------------------------------------------------
    void luaL_warn( lua_State *L, const char *fmt, ... ){
        va_list argp;
        va_start( argp, fmt );
        luaL_where( L, 1 );
        lua_pushvfstring( L, fmt, argp );
        va_end( argp );
        lua_concat( L, 2 );
        Util::LOGW( "Lua: ", lua_tostring( L, -1 ) );
        lua_pop( L, 1 );
    }

    //--------------------------------------------------------------------------
    class DriverInfo{
    public:
        string driverName;
        vector<string> driverExts;
    };

    const vector<DriverInfo> s_driverNames = {
        {"BMP", {".bmp",".dib"} },
        {"GIF", {".gif"} },
        {"JPEG", {".jpg", ".jpeg"} },
        {"PNG", {".png"} },
        {"GTiff", {".tiff",".tif"}},
    };

    //--------------------------------------------------------------------------
    unique_ptr<string> getGDALDriverName( const string & filename ){
        using Value = string::value_type;
        auto lower = [](const Value c){ return (Value)tolower(c); };

        // get the lowercase extension of filename
        auto fileext = path( filename ).extension().generic_string();
        string normext;
        normext.resize( fileext.size() );
        transform( begin(fileext), end(fileext), begin(normext), lower );

        // search the extension in the driver list
        for( const auto di: s_driverNames ){
            auto el = find( begin(di.driverExts), end(di.driverExts), normext);
            if( el != end(di.driverExts) ){
                return unique_ptr<string>(new string(di.driverName));
            }
        }

        return nullptr;
    }
}

//------------------------------------------------------------------------------
