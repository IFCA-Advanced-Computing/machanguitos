//------------------------------------------------------------------------------
#include "agentfactory.h"

#include <cassert>
#include <iostream>

#include <boost/filesystem.hpp>

#include "lua.hpp"

#include "agentclass.h"

//------------------------------------------------------------------------------
namespace Agent{
    using namespace boost::filesystem;
    using namespace std;

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::createClass( const string & name ){
        auto el = m_classes.find( name );
        if( el != m_classes.end() ){
            return el->second;
        }

        path filename = path(m_dir) /= (name + ".lua");
        if( !is_regular_file(filename) ){
            cerr << "ERROR: not file for class " << name << endl;
            return nullptr;
        }

        // Lua Initialization
        auto L = luaL_newstate();
        assert( L && "Can't create Lua State" );

        lua_gc( L, LUA_GCSTOP, 0 );
        luaL_openlibs( L );
        lua_gc( L, LUA_GCRESTART, 0 );

        // create Agent class on Lua
        lua_newtable( L );
        lua_setfield(L, LUA_GLOBALSINDEX, "Agent");

        // execute class file
        auto ret = luaL_dofile( L, filename.c_str() );
        if( ret != 0 ){
            auto msg = lua_tostring(L, -1);
            if( msg == nullptr ){
                cerr << "Error : (error object is not a string)\n";
            }else{
                cerr << msg << endl;
            }
            lua_close( L );
            return nullptr;
        }

        AgentClass * aclass = new AgentClass( L );
        m_classes[name] = aclass;

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
