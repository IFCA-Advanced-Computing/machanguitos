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
    /** Singleton Class that controls teh simulation execution.
        @ingroup Engine
     */
    class Server : public Singleton<Server> {
    public:
        /** Add a Client in the simulation.
            @param c Client instance.
         */
        void addClient( Client * c );
        /** Add agents number to create them in the future.

            This method don't create the agent rigth now. It only adds to an
            internal list. It's possible to execute several times with the same
            name. E.g:

            \code
            Server::instance()->addAgents( "cow", 10 );
            Server::instance()->addAgents( "cow", 10 );
            \endcode

            Has the same result of:
          
            \code
            Server::instance()->addAgents( "cow", 20 );
            \endcode

            @param name of the AgentClass.
            @param n number of Agents to create.
         */
        void addAgents( const std::string & name, const unsigned n );
        /// create the agents for the simulation.
        void createAgents();
        /** Insert a config parameter.
            @param key name of the parameter.
            @param val value of the parameter.
         */
        void insertConfig( const std::string & key, Util::ScriptValue && val );
        /// run loop of the simulation.
        void run();

    private:
        /** Returns the Integer value of a config parameter.
            @param key name of the parameter.
            @param d default value.
            @returns the value, or default if this variable is not integer type.
         */
        int getConfigInt( const std::string & key, const int d ) const;
        /** Returns the numeric value of a config parameter.
            @param key name of the parameter.
            @param d default value.
            @returns the value, or default if this variable is not numeric type.
         */
        double getConfigNumber( const std::string & key, const double d ) const;

        /// list of number of agents to create of each AgentClass.
        std::map<std::string, unsigned> m_numAgents;
        /// list of Clients used during simulation.
        std::vector<Client*> m_clients;
        /// parameters loaded from config file.
        std::map<std::string, Util::ScriptValue> m_config;
    };

    //--------------------------------------------------------------------------
    inline void Server::insertConfig( const std::string & key, 
                                      Util::ScriptValue && val ){
        m_config[key] = std::move(val);
    }
}

//------------------------------------------------------------------------------
#endif//SERVER_H

//------------------------------------------------------------------------------
