//------------------------------------------------------------------------------
#ifndef SERVER_H
#define SERVER_H

//------------------------------------------------------------------------------
#include <string>
#include <map>

#include "singleton.h"

//------------------------------------------------------------------------------
namespace Engine{
    class Server : public Singleton<Server> {
    public:
        void addAgents( const std::string & name, const int n );
        void createAgents();
        void run();

    private:
        std::map<std::string, int> m_numAgents;
    };
}

//------------------------------------------------------------------------------
#endif//SERVER_H

//------------------------------------------------------------------------------
