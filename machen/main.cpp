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
#include <mpi.h>
#include "common/log.hpp"
#include "config.h"
#include "configlib.hpp"
#include "server.hpp"
#include "mpidefs.hpp"
#include "mpiworker.hpp"
#include "mpidataserver.hpp"
#include "engine.hpp"

//------------------------------------------------------------------------------
using namespace std;

//------------------------------------------------------------------------------
/** Output application how to message
    @param name application name
 */
void printHelp( const string & name ){
    cerr << name << " " << VERSION_MAJOR << "." << VERSION_MINOR
              << endl << endl;
    cerr << "Usage: " << name << " <config.lua>\n\n";
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

    Engine::initialize( argc, argv );

    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    // MPI SERVER
    if( rank == Engine::SERVER_RANK ){
        if( argc != 2 ){
            printHelp( argv[0] );
            return Engine::abort();
        }

        string filename{ argv[1] };
        if( !Config::load( filename ) ){
            return Engine::abort();
        }

        Util::LOGV( "Creating Server " );
        auto server = Engine::Server::instance();
        server->setDatadir( filename );
        if( !server->initialize() ){
            return Engine::abort();
        }
        server->createAgents();
        server->run();

    // MPI DATA SERVER
    }else if( rank == Engine::DATASERVER_RANK ) {
        Engine::MPIDataServer dserver;
        dserver.run();

    // MPI CLIENTS
    }else{
        Engine::MPIWorker worker(rank);
        worker.run();
    }

    Engine::end();

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
