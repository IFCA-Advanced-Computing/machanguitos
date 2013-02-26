//------------------------------------------------------------------------------
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "configlib.h"
#include "server.h"
#include "clientlocal.h"

#if defined(HAVE_MPI)
#include "mpi.h"

//------------------------------------------------------------------------------
int main( int argc, char * argv[] ){
    int my_rank, numprocs;
    // MPI initializations
    // MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );
    double time_start = MPI_Wtime();
    std::cout << "Hello World, my rank is " << my_rank << " "
              << MPI_Wtime() - time_start << std::endl;
    // End MPI
    MPI_Finalize();
    return EXIT_SUCCESS;
}

#else//!HAVE_MPI

//------------------------------------------------------------------------------
int main( const int argc, const char * argv[] ){
    assert( argc > 1 && "Invalid number of arguments" );

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

#endif//HAVE_MPI

//------------------------------------------------------------------------------
