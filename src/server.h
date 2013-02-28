//------------------------------------------------------------------------------
#ifndef SERVER_H
#define SERVER_H

//------------------------------------------------------------------------------
#include <string>
#include <map>
#include <vector>

#include "singleton.h"
#include "scriptvalue.h"

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    class Client;

    //--------------------------------------------------------------------------
    class Server : public Singleton<Server> {
    public:
        void addClient( Client * c );
        void addAgents( const std::string & name, const unsigned n );
        void createAgents();
        void insertConfig( const std::string & key, Util::ScriptValue && val );
        void run();

    private:
        int getConfigInt( const std::string & key, const int d ) const;
        double getConfigNumber( const std::string & key, const double d ) const;

        std::map<std::string, unsigned> m_numAgents;
        std::vector<Client*> m_clients;
        std::map<std::string, Util::ScriptValue> m_config;
    };

    //--------------------------------------------------------------------------
    inline void Server::insertConfig( const std::string & key, Util::ScriptValue && val ){
        m_config[key] = std::move(val);
    }
}

//------------------------------------------------------------------------------
#endif//SERVER_H

//------------------------------------------------------------------------------
