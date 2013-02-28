//------------------------------------------------------------------------------
#include "clientremote.h"

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cstring>

#include "config.h"

#if defined(HAVE_MPI)
#include <mpi.h>
#include "mpitags.h"
#endif//HAVE_MPI


//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    ClientRemote::ClientRemote( int dest ) : m_dest{dest} {
        // empty
    }

    //--------------------------------------------------------------------------
    ClientRemote::~ClientRemote(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool ClientRemote::createClass( const std::string & name ){
#if defined(HAVE_MPI)
        assert( name.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val{0};
        char * cstr = new char [name.length()+1];
        strcpy( cstr, name.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, TAG_CREATECLASS, MPI_COMM_WORLD );
        MPI_Send( cstr, name.length(), MPI_CHAR, m_dest, TAG_CREATECLASS, MPI_COMM_WORLD );
        return true;
#else//!HAVE_MPI
        return false;
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void ClientRemote::createAgents( const std::string & name, int n ){
#if defined(HAVE_MPI)
        assert( name.length() <= MAX_CLASS_NAME && "name too long" );
        int32_t val = n;
        char * cstr = new char [name.length()+1];
        strcpy( cstr, name.c_str() );

        MPI_Send( &val, 1, MPI_INT, m_dest, TAG_CREATEAGENTS, MPI_COMM_WORLD );
        MPI_Send( cstr, name.length(), MPI_CHAR, m_dest, TAG_CREATEAGENTS, MPI_COMM_WORLD );

        m_numAgents += n;
#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void ClientRemote::runAgents( const double delta ){
#if defined(HAVE_MPI)
        int32_t val{0};
        double d{delta};
        MPI_Send( &val, 1, MPI_INT, m_dest, TAG_RUNAGENTS, MPI_COMM_WORLD );
        MPI_Send( &d, 1, MPI_DOUBLE, m_dest, TAG_RUNAGENTS, MPI_COMM_WORLD );
#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void ClientRemote::end(){
#if defined(HAVE_MPI)
        int32_t val{0};
        MPI_Send( &val, 1, MPI_INT, m_dest, TAG_END, MPI_COMM_WORLD);
#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }


}

//------------------------------------------------------------------------------
