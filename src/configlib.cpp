#include "configlib.h"

#include <iostream>

#include "lua.hpp"

namespace Config{

    int config_add_agent( lua_State *L ){
        auto name = luaL_checkstring( L, 1 );
        auto num = luaL_checknumber( L, 2 );
        std::cout << "Creating: " << num << " of " << name << std::endl;
        return 0;
    }

    int config_setvars( lua_State *L ){
        luaL_checktype( L, 1, LUA_TTABLE );
        // push first key
        lua_pushnil( L );
        while( lua_next( L, 1 ) != 0 ) {
            std::string keyname( luaL_checkstring( L, -2 ) );
            std::cout << keyname << " = " 
                      << lua_typename(L, lua_type(L, -1)) << std::endl;
            // removes 'value'; keeps 'key' for next iteration
            lua_pop( L, 1 );
        }
        return 0;
    }

    const luaL_Reg configlib[] = {
        {"add_agent",   config_add_agent},
        {"setvars",   config_setvars},
        {nullptr, nullptr}
    };

    int openlib( lua_State *L ){
        // set functions
        luaL_register( L, "config", configlib );
        // removes table
        lua_pop( L, 1 );
        return 0;
    }
}
