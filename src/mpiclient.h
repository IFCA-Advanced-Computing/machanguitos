//------------------------------------------------------------------------------
#ifndef MPICLIENT_H
#define MPICLIENT_H

namespace Engine{
    /** Define a MPI Process main loop to run as a client in a server/client
        configuration.

        @ingroup Engine
     */
    class Client;

    class MPIClient {
    public:
        MPIClient( const int r );
        ~MPIClient();

        void run();

    private:
        void runCreateClass();
        void runCreateAgents( const int num );

        int m_rank;
        Client * m_local{nullptr};
    };
}

//------------------------------------------------------------------------------
#endif//MPICLIENT_H

//------------------------------------------------------------------------------
