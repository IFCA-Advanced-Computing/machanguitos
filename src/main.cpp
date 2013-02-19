//------------------------------------------------------------------------------
#include <cstdlib>
#include <cassert>
#include <iostream>
 
#include "configlib.h"
#include "server.h"

//------------------------------------------------------------------------------
int main( const int argc, const char * argv[] ){
    assert( argc > 1 && "Invalid number of arguments" );

    if( !Config::load( argv[1] ) ){
        return EXIT_FAILURE;
    }

    auto server = Engine::Server::instance();

    server->createAgents();
    server->run();

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
