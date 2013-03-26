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
/** @file clientremote.cpp
    @brief Engine::ClientRemote Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "clientremote.hpp"
#include <cstdint>
#include <iostream>
#include <cassert>
#include <cstring>
#include <mpi.h>
#include "config.h"
#include "mpitags.hpp"


//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    ClientRemote::ClientRemote( int dest ) : m_dest{dest}, m_numAgents{0} {
        // empty
    }

    //--------------------------------------------------------------------------
    ClientRemote::~ClientRemote(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool ClientRemote::createClass( const std::string & name ){
        assert( name.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val{0};
        char * cname = new char [name.length()+1];
        strcpy( cname, name.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::CREATECLASS, MPI_COMM_WORLD );
        MPI_Send( cname, name.length(), MPI_CHAR, m_dest, MpiTag::CREATECLASS, MPI_COMM_WORLD );
        return true;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::setStartTime( const double time ){
        int32_t val{0};
        double t{time};
        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::SETSTARTTIME, MPI_COMM_WORLD );
        MPI_Send( &t, 1, MPI_DOUBLE, m_dest, MpiTag::SETSTARTTIME, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void ClientRemote::setDataDir( const std::string & filename ){
        assert( filename.length() <= MAX_PATH_NAME && "filename too long" );
        int32_t val{0};
        char * cname = new char [filename.length()+1];
        strcpy( cname, filename.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::SETDATAPATH, MPI_COMM_WORLD );
        MPI_Send( cname, filename.length(), MPI_CHAR, m_dest, MpiTag::SETDATAPATH, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void ClientRemote::setDataStore( const std::string & name,
                                     const std::string & host, const uint16_t port )
    {
        assert( name.length() <= MAX_DB_NAME && "name too long" );
        assert( name.length() <= MAX_HOST_NAME && "hostname too long" );
        int32_t val{port};
        char * cname = new char [name.length()+1];
        strcpy( cname, name.c_str() );
        char * chost = new char [host.length()+1];
        strcpy( chost, host.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::SETDATASTORE, MPI_COMM_WORLD );
        MPI_Send( cname, name.length(), MPI_CHAR, m_dest, MpiTag::SETDATASTORE, MPI_COMM_WORLD );
        MPI_Send( chost, host.length(), MPI_CHAR, m_dest, MpiTag::SETDATASTORE, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void ClientRemote::createAgents( const std::string & name, int n ){
        assert( name.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val = n;
        char * cname = new char [name.length()+1];
        strcpy( cname, name.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::CREATEAGENTS, MPI_COMM_WORLD );
        MPI_Send( cname, name.length(), MPI_CHAR, m_dest, MpiTag::CREATEAGENTS, MPI_COMM_WORLD );

        m_numAgents += n;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::runAgents( const double delta ){
        int32_t val{0};
        double d{delta};
        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::RUNAGENTS, MPI_COMM_WORLD );
        MPI_Send( &d, 1, MPI_DOUBLE, m_dest, MpiTag::RUNAGENTS, MPI_COMM_WORLD );
    }

    //--------------------------------------------------------------------------
    void ClientRemote::end(){
        int32_t val{0};
        MPI_Send( &val, 1, MPI_INT, m_dest, MpiTag::END, MPI_COMM_WORLD);
    }

}

//------------------------------------------------------------------------------
