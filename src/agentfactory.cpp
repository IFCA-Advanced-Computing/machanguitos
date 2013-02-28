//------------------------------------------------------------------------------
#include "agentfactory.h"

#include <cassert>
#include <iostream>

#include <boost/filesystem.hpp>

#include "lua.hpp"

#include "util.h"
#include "agentclass.h"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace boost::filesystem;
    using namespace std;
    using namespace Util;

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::createClass( const string & name ){
        auto el = m_classes.find( name );
        if( el != m_classes.end() ){
            return el->second;
        }

        path filename = path(m_dir) /= (name + ".lua");
        if( !is_regular_file(filename) ){
            cerr << "ERROR: not file for class '" << name << "'\n";
            return nullptr;
        }

        // Lua Initialization
        auto L = luaL_newstate();
        if( !L ){
            cerr << "ERROR: Can't create Lua State\n";
            return nullptr;
        }

        lua_gc( L, LUA_GCSTOP, 0 );
        luaL_openlibs( L );
        lua_gc( L, LUA_GCRESTART, 0 );

        // create Agent class on Lua
        lua_newtable( L );
        lua_setfield(L, LUA_GLOBALSINDEX, SCRIPT_AGENT_NAME );

        // execute class file
        auto ret = luaL_dofile( L, filename.c_str() );
        if( !checkLuaReturn( L, ret ) ){
            lua_close( L );
            return nullptr;
        }

        AgentClass * aclass = new (std::nothrow) AgentClass( L );
        if( aclass ){
            m_classes[name] = aclass;
        }

        return aclass;
    }

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::getClass( const string & name ) const{
        auto el = m_classes.find( name );
        if( el != m_classes.end() ){
            return el->second;
        }
        return nullptr;
    }

    //--------------------------------------------------------------------------
    void AgentFactory::setDatadir( const string & dir ){
        assert( is_directory(dir) && "not directory" );
        m_dir = dir;        
    }
}

//------------------------------------------------------------------------------
