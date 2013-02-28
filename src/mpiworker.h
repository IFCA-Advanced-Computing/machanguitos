//------------------------------------------------------------------------------
#ifndef MPIWORKER_H
#define MPIWORKER_H

namespace Engine{
    /** Define a MPI Process main loop to run as a Worker in a MPI program.

        @ingroup Engine
     */
    class Client;

    class MPIWorker {
    public:
        MPIWorker( const int r );
        ~MPIWorker();

        void run();

    private:
        void runCreateClass();
        void runCreateAgents( const int num );
        void runAgents();

        int m_rank;
        Client * m_local{nullptr};
    };
}

//------------------------------------------------------------------------------
#endif//MPIWORKER_H

//------------------------------------------------------------------------------
