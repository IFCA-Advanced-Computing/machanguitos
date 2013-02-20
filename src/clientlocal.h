//------------------------------------------------------------------------------
#ifndef CLIENTLOCAL_H
#define CLIENTLOCAL_H

#include "client.h"

//------------------------------------------------------------------------------
namespace Engine{
    class ClientLocal : public Client{
    public:
        ClientLocal();
        virtual ~ClientLocal();

        bool createClass( const std::string & name ) override;
        void createAgents( const std::string & name, int n ) override;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENTLOCAL_H

//------------------------------------------------------------------------------
