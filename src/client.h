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
        virtual void runAgents( const double delta )=0;
        virtual int numAgents() const =0;
        virtual void end()=0;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENT_H

//------------------------------------------------------------------------------
