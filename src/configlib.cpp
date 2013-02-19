//------------------------------------------------------------------------------
#include "configlib.h"

#include <iostream>
#include <cassert>

#include "lua.hpp"

#include "config.h"
#include "server.h"

//------------------------------------------------------------------------------
namespace Config{

    //--------------------------------------------------------------------------
    int config_add_agent( lua_State *L ){
        auto name = luaL_checkstring( L, 1 );
        auto num = luaL_checknumber( L, 2 );
        auto server = Engine::Server::instance();
        server->addAgents( name, num );
        return 0;
    }

    //--------------------------------------------------------------------------
    int config_setvars( lua_State *L ){
        luaL_checktype( L, 1, LUA_TTABLE );
        // push first key
        lua_pushnil( L );
        while( lua_next( L, 1 ) != 0 ) {
            std::string keyname( luaL_checkstring( L, -2 ) );
            std::cout << keyname << " = ";
            auto ltype = lua_type( L, -1 );
            switch( ltype ){
            case LUA_TNUMBER:
                std::cout << lua_tonumber(L, -1 ) << std::endl;
                break;

            case LUA_TSTRING:
                std::cout << "'" << lua_tostring(L, -1 ) << "'" << std::endl;
                break;

            default:
                std::cout << lua_typename(L, ltype ) << std::endl;
            }

            // removes 'value'; keeps 'key' for next iteration
            lua_pop( L, 1 );
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    const luaL_Reg configlib[] = {
        {"add_agent",   config_add_agent},
        {"setvars",   config_setvars},
        {nullptr, nullptr}
    };

    //--------------------------------------------------------------------------
    int openlib( lua_State *L ){
        // set functions
        luaL_register( L, "config", configlib );
        // Set constants
        lua_pushnumber( L, VERSION_MAJOR );
        lua_setfield( L, -2, "VERSION_MAJOR" );
        lua_pushnumber( L, VERSION_MINOR );
        lua_setfield( L, -2, "VERSION_MINOR" );
        // removes table
        lua_pop( L, 1 );
        return 0;
    }

    //--------------------------------------------------------------------------
    bool load( const std::string & filename ){
        bool is_ok{true};
        auto L = luaL_newstate();
        assert( L && "Can't create Lua State" );

        // Lua Initialization
        lua_gc(L, LUA_GCSTOP, 0);
        luaL_openlibs( L );
        Config::openlib( L );
        lua_gc(L, LUA_GCRESTART, 0);

        auto ret = luaL_dofile( L, filename.c_str() );
        if( ret != 0 ){
            auto msg = lua_tostring(L, -1);
            if( msg == nullptr ){
                std::cout << "Error : (error object is not a string)\n";
            }else{
                std::cout << msg << std::endl;
            }
            is_ok = false;
        }

        lua_close( L );

        return is_ok;
    }
}

//------------------------------------------------------------------------------
