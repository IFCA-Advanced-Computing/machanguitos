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
/** @file engine.cpp
    @brief Engine common function definitions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "engine.hpp"
#include <boost/filesystem.hpp>
#include "common/mpi.hpp"
#include "mpidefs.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    using namespace boost::filesystem;
    using namespace std;

    //--------------------------------------------------------------------------
    /// minimum number of MPI proccess needed to run.
    constexpr int MIN_MPI_PROCS = 3;

    //--------------------------------------------------------------------------
    /// Clients communicator.
    MPI_Comm m_clientsComm = MPI_COMM_WORLD;
    /// rank of current process.
    int m_rank;
    /// number of mpi processes.
    int m_nprocs;
    /// Data Files directory.
    std::string m_datadir{""};

    //--------------------------------------------------------------------------
    int abort(){
        MPI_Abort( MPI_COMM_WORLD, EXIT_FAILURE );
        return EXIT_FAILURE;
    }

    //--------------------------------------------------------------------------
    void clientsBarrier(){
        MPI_Barrier( m_clientsComm );
    }

    //--------------------------------------------------------------------------
    bool initialize( int argc, char * argv[] ){
        MPI_Init( &argc, &argv );
        int initflag;
        MPI_Initialized( &initflag );
        if( not initflag ){
            Util::LOGE( "Error Initializing MPI" );
            Util::LOGE( "Error  Did you use mpirun?" );
            return false;
        }

        MPI_Comm_size( MPI_COMM_WORLD, &m_nprocs );
        MPI_Comm_rank( MPI_COMM_WORLD, &m_rank );

        if( m_nprocs < MIN_MPI_PROCS ){
            Util::LOGE( "Erroneous number of process. Minimum : ", MIN_MPI_PROCS );
            return false;
        }

        if( m_nprocs > 1 ){
            MPI_Group MPI_GROUP_WORLD, clients;
            int datarank = DATASERVER_RANK;

            MPI_Comm_group( MPI_COMM_WORLD, &MPI_GROUP_WORLD );
            MPI_Group_excl( MPI_GROUP_WORLD, 1, &datarank, &clients );

            MPI_Comm_create( MPI_COMM_WORLD, clients, &m_clientsComm );
        }
        return true;
    }

    //--------------------------------------------------------------------------
    void end(){
        MPI_Finalize();
    }

    //--------------------------------------------------------------------------
    bool isServer(){
        return m_rank == Engine::SERVER_RANK;
    }

    //--------------------------------------------------------------------------
    bool isDataServer(){
        return m_rank == Engine::DATASERVER_RANK;
    }

    //--------------------------------------------------------------------------
    bool isSingleProcess(){
        return m_nprocs == 1;
    }

    //--------------------------------------------------------------------------
    void setDataDir( const string & dir ){
        if( is_directory(dir) ){
            m_datadir = dir;
        }else{
            boost::filesystem::path datadir( dir );
            datadir.remove_filename();
            m_datadir = datadir.c_str();
        }
    }

    //--------------------------------------------------------------------------
    int getNumProcesses(){
        return m_nprocs;
    }

    //--------------------------------------------------------------------------
    const string & getDataDir(){
        return m_datadir;
    }

//------------------------------------------------------------------------------
}//namespace Engine

//------------------------------------------------------------------------------
