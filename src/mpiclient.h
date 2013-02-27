//------------------------------------------------------------------------------
#ifndef MPICLIENT_H
#define MPICLIENT_H

namespace Engine{
    /** Define a MPI Process main loop to run as a client in a server/client
        configuration.

        @ingroup Engine
     */
    class MPIClient {
    public:
        MPIClient( const int r );

        void run();

    private:
        void runCreateClass();

        int m_rank;
    };

    //--------------------------------------------------------------------------
    inline MPIClient::MPIClient( const int r ) : m_rank{r} {
        // empty
    }
}

//------------------------------------------------------------------------------
#endif//MPICLIENT_H

//------------------------------------------------------------------------------
