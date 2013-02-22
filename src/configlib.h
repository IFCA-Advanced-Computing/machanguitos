#ifndef CONFIGLIB_H
#define CONFIGLIB_H

#include <string>

struct lua_State;

namespace Config{
    bool load( const std::string & filename );
}

#endif//CONFIGLIB_H
