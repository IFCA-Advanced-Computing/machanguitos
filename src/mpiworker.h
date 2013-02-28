//------------------------------------------------------------------------------
#ifndef MPIWORKER_H
#define MPIWORKER_H

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    class Client;

    //--------------------------------------------------------------------------
    /** Define a MPI Process main loop to run as a Worker in a MPI program.

        @ingroup Engine
     */
    class MPIWorker {
    public:
        /** Construct a MPI worker
            @param r MPI rank
        */
        MPIWorker( const int r );
        ~MPIWorker();

        /// run loop of the MPI worker waiting for Server orders.
        void run();

    private:
        /// Execute a Create Class command.
        void runCreateClass();
        /** Execute a Create Agents command.
            @param num number of agents to create.
        */
        void runCreateAgents( const int num );
        /// Execute a End command.
        void runAgents();

        /// Own MPI rank value.
        int m_rank;
        /// Client instance that runs the real code.
        Client * m_local;
    };
}

//------------------------------------------------------------------------------
#endif//MPIWORKER_H

//------------------------------------------------------------------------------
