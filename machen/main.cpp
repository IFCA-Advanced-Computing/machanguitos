/*******************************************************************************
Machanguitos is The Easiest Multi-Agent System in the world. Work done at The
Institute of Physics of Cantabria (IFCA).
Copyright (C) 2013  Luis Cabellos

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
/** @file main.cpp
    @brief Main function of machen application.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include <cstdlib>
#include <cassert>
#include <boost/filesystem.hpp>
#include "config.h"
#include "configlib.hpp"
#include "agentfactory.hpp"
#include "server.hpp"
#include "clientlocal.hpp"
#include "mpi.h"
#include "clientremote.hpp"
#include "mpiworker.hpp"

//------------------------------------------------------------------------------
/** Output application how to message
    @param name application name
 */
void printHelp( const std::string & name ){
    std::cerr << name << " " << VERSION_MAJOR << "." << VERSION_MINOR
              << std::endl << std::endl;
    std::cerr << "Usage: " << name << " <config.lua>\n\n";
}

//------------------------------------------------------------------------------
/** Main application function.
    @param argc argument count.
    @param argv argument vector.
    @returns exit status of the process.
 */
int main( int argc, char * argv[] ){
    static_assert( sizeof(double) == sizeof(int64_t),
                   "Double type isn't 64 bits" );
    MPI_Init( &argc, &argv );

    int nprocs, rank;

    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    // MPI SERVER
    if( rank == 0 ){
        if( argc != 2 ){
            assert( argc > 0 && "Error in command args" );
            printHelp( argv[0] );
            MPI_Abort( MPI_COMM_WORLD, EXIT_FAILURE );
            return EXIT_FAILURE;
        }

        std::string filename{ argv[1] };
        if( !Config::load( filename ) ){
            MPI_Abort( MPI_COMM_WORLD, EXIT_FAILURE );
            return EXIT_FAILURE;
        }

        // set directory for Agent classes
        boost::filesystem::path datadir( filename );
        datadir.remove_filename();
        Agent::AgentFactory::instance()->setDatadir( datadir.c_str() );

        auto server = Engine::Server::instance();

        if( nprocs == 1 ){
            Engine::Client * client = new Engine::ClientLocal( 0 );
            server->addClient( client );
        }

        for( int i = 1 ; i < nprocs ; ++i ){
            Engine::Client * client = new Engine::ClientRemote( i );
            server->addClient( client );
        }

        if( !server->initialize() ){
            MPI_Abort( MPI_COMM_WORLD, EXIT_FAILURE );
            return EXIT_FAILURE;
        }
        server->createAgents();
        server->run();

    // MPI CLIENTS
    }else{
        Engine::MPIWorker worker(rank);

        worker.run();
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
