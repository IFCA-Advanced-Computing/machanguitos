//------------------------------------------------------------------------------
#ifndef CLIENTREMOTE_H
#define CLIENTREMOTE_H

#include "client.h"

//------------------------------------------------------------------------------
namespace Engine{
    /** @ingroup: Engine
     */
    class ClientRemote : public Client{
    public:
        ClientRemote( int dest );
        virtual ~ClientRemote();

        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
        void runAgents( const double delta ) override;
        void end() override;

    private:
        int m_dest;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENTREMOTE_H

//------------------------------------------------------------------------------
