//------------------------------------------------------------------------------
#ifndef CLIENT_H
#define CLIENT_H

//------------------------------------------------------------------------------
#include <string>

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    /** Client Interface.

        A Client is the class that should maintain the Agents and execute its
        code.

        @ingroup Engine
     */
    class Client {
    public:
        Client();
        virtual ~Client();

        /** Create a AgentClass and save it.
            @param name name of the AgentClass.
            @retval true if it can create the AgentClass instance.
         */
        virtual bool createClass( const std::string & name )=0;
        /** Create Agents of an AgentClass instance.
            @param name name of the AgentClass.
            @param n number of agents to create.
         */
        virtual void createAgents( const std::string & name, int n )=0;
        /** Run a tick of the 'update' function for the Agents in this Client
            instance.  
            @param delta time increment in seconds between updates.
         */
        virtual void runAgents( const double delta )=0;
        /// Returns the number of agents in this Client instance.
        virtual int numAgents() const =0;
        /// End the Agent simulation.
        virtual void end()=0;
    };
}

//------------------------------------------------------------------------------
#endif//CLIENT_H

//------------------------------------------------------------------------------
