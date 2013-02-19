#include <cstdlib>
#include <cassert>
#include <iostream>
 
#include "configlib.h"

int main( const int argc, const char * argv[] ){
    assert( argc > 1 && "Invalid number of arguments" );

    if( !Config::load( argv[1] ) ){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
