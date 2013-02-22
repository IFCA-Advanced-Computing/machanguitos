//------------------------------------------------------------------------------
#include "util.h"

#include <iostream>

#include "lua.hpp"

//------------------------------------------------------------------------------
namespace Util{
    using namespace std;

    //--------------------------------------------------------------------------
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
}

//------------------------------------------------------------------------------
