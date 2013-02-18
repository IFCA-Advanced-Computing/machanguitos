#include <cstdlib>
#include <cassert>
#include <iostream>
 
#include "lua.hpp"

#include "configlib.h"

int main( const int argc, const char * argv[] ){
    assert( argc > 1 && "Invalid number of arguments" );

    lua_State * L = luaL_newstate();
    assert( L && "Can't create Lua State" );
    
    // Lua Initialization
    lua_gc(L, LUA_GCSTOP, 0);
    luaL_openlibs( L );
    Config::openlib( L );
    lua_gc(L, LUA_GCRESTART, 0);
    
    auto ret = luaL_dofile( L, argv[1] );
    if( ret != LUA_OK ){
        auto msg = lua_tostring(L, -1);
        if( msg == nullptr ){
            std::cout << "Error : (error object is not a string)\n";
        }else{
            std::cout << msg << std::endl;
        }
    }
    
    lua_close( L );

    return EXIT_SUCCESS;
}
