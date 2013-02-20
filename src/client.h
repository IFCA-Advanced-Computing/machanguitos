//------------------------------------------------------------------------------
#ifndef CLIENT_H
#define CLIENT_H

#include <string>

//------------------------------------------------------------------------------
namespace Engine{
    class Client {
    public:
        Client();
        virtual ~Client();

        virtual bool createClass( const std::string & name )=0;
        virtual void createAgents( const std::string & name, int n )=0;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENT_H

//------------------------------------------------------------------------------

