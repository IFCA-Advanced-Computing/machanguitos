#include "configlib.h"

#include "lua.hpp"

namespace Config{

    int config_add_agent( lua_State *L ){
        return 0;
    }

    int config_setvars( lua_State *L ){
        return 0;
    }

    const luaL_Reg configlib[] = {
        {"add_agent",   config_add_agent},
        {"setvars",   config_setvars},
        {nullptr, nullptr}
    };

    int openlib( lua_State *L ){
        luaL_newlib( L, configlib );
        /* HOW to set constants
        lua_pushnumber( L, PI );
        lua_setfield( L, -2, "pi" );*/
        lua_setglobal(L, "config");
        return 0;
    }
}
