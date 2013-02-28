//------------------------------------------------------------------------------
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "config.h"
#include "configlib.h"
#include "server.h"
#include "clientlocal.h"

//------------------------------------------------------------------------------
void printHelp( const std::string & name ){
    std::cerr << name << " " << VERSION_MAJOR << "." << VERSION_MINOR
              << std::endl << std::endl;
    std::cerr << "Usage: " << name << " <config.lua>\n\n";
}

//------------------------------------------------------------------------------
int singleMain( int argc, char * argv[] ){
    if( argc != 2 ){
        assert( argc > 0 && "Error in command args" );
        printHelp( argv[0] );
        return EXIT_FAILURE;
    }

    if( !Config::load( argv[1] ) ){
        return EXIT_FAILURE;
    }

    auto server = Engine::Server::instance();

    Engine::Client * client = new Engine::ClientLocal();

    server->addClient( client );
    server->createAgents();
    server->run();

    return EXIT_SUCCESS;
}

#if defined(HAVE_MPI)
#include "mpi.h"
#include "clientremote.h"
#include "mpiworker.h"

//------------------------------------------------------------------------------
int multiMain( int argc, char * argv[] ){
    int nprocs, rank;

    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if( rank == 0 ){
        if( argc != 2 ){
            assert( argc > 0 && "Error in command args" );
            printHelp( argv[0] );
            return EXIT_FAILURE;
        }

        if( !Config::load( argv[1] ) ){
            return EXIT_FAILURE;
        }

        auto server = Engine::Server::instance();

        for( int i = 1 ; i < nprocs ; ++i ){
            Engine::Client * client = new Engine::ClientRemote( i );
            server->addClient( client );
        }

        server->createAgents();
        server->run();

    }else{
        if( argc != 2 ){
            return EXIT_FAILURE;
        }

        if( !Config::setDataDir( argv[1] ) ){
            return EXIT_FAILURE;
        }

        Engine::MPIWorker worker(rank);

        worker.run();
    }

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
int main( int argc, char * argv[] ){
    MPI_Init( &argc, &argv );

    int nprocs;
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );

    int ret{ EXIT_SUCCESS };
    if( nprocs == 1 ){
        ret = singleMain( argc, argv );
    }else{
        ret = multiMain( argc, argv );
    }

    MPI_Finalize();

    return ret;
}

#else//!HAVE_MPI

//------------------------------------------------------------------------------
int main( const int argc, const char * argv[] ){
    return singleMain( argc, argv );
}

#endif//HAVE_MPI

//------------------------------------------------------------------------------
