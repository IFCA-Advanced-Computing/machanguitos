//------------------------------------------------------------------------------
#include "mpiclient.h"

#include "config.h"
#if defined(HAVE_MPI)
#include <mpi.h>
#include "mpitags.h"
#endif
//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    void MPIClient::run(){
#if defined(HAVE_MPI)
        int32_t val{0};
        MPI_Status status;
        bool running = true;

        while( running ){
            MPI_Recv( &val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
            if( status.MPI_ERROR != MPI_SUCCESS ){
                std::cerr << "ERROR: Received on " << m_rank << std::endl;
                MPI_Abort( MPI_COMM_WORLD, 0 );
            }

            switch( status.MPI_TAG ){
            case TAG_CREATECLASS:
                runCreateClass();
                break;

            case TAG_END:
                running = false;
                break;

            default:
                    std::cerr << "ERROR: not-implemented message[" << status.MPI_TAG 
                              << "] on " << m_rank << std::endl;
            }
        }
        
#else//!HAVE_MPI
        assert( false && "MPI code without MPI" );
#endif//HAVE_MPI
    }

    //--------------------------------------------------------------------------
    void MPIClient::runCreateClass(){
        std::cout << " creating class " << std::endl;
    }
}

//------------------------------------------------------------------------------
