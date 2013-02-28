//------------------------------------------------------------------------------
#ifndef CLIENTREMOTE_H
#define CLIENTREMOTE_H

#include "client.h"

//------------------------------------------------------------------------------
namespace Engine{
    /** Client instance that acts as proxy of a remote MPI worker.
        @ingroup: Engine
     */
    class ClientRemote : public Client{
    public:
        /** Constructor.
            @param dest MPI rank of remote worker.
         */
        ClientRemote( int dest );
        virtual ~ClientRemote();

        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
        void runAgents( const double delta ) override;
        int numAgents() const override;
        void end() override;

    private:
        /// MPI rank of remote worker.
        int m_dest;
        /// number of Agents created in remote.
        int m_numAgents;
    };

    //--------------------------------------------------------------------------
    inline int ClientRemote::numAgents() const{
        return m_numAgents;
    }
}

//------------------------------------------------------------------------------
#endif//CLIENTREMOTE_H

//------------------------------------------------------------------------------
