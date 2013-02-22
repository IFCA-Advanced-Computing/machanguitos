//------------------------------------------------------------------------------
#ifndef SERVER_H
#define SERVER_H

//------------------------------------------------------------------------------
#include <string>
#include <map>
#include <list>

#include "singleton.h"

//------------------------------------------------------------------------------
namespace Engine{
    class Client;

    class Server : public Singleton<Server> {
    public:
        void addClient( Client * c );
        void addAgents( const std::string & name, const unsigned n );
        void createAgents();
        void run();

    private:
        std::map<std::string, unsigned> m_numAgents;
        std::list<Client*> m_clients;
    };
}

//------------------------------------------------------------------------------
#endif//SERVER_H

//------------------------------------------------------------------------------
