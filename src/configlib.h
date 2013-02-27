#ifndef CONFIGLIB_H
#define CONFIGLIB_H

#include <string>

struct lua_State;

namespace Config{
    bool setDataDir( const std::string & filename );
    bool load( const std::string & filename );
}

#endif//CONFIGLIB_H
