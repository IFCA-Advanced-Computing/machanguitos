//------------------------------------------------------------------------------
#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

//------------------------------------------------------------------------------
#include "singleton.h"

#include <string>
#include <map>

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    class AgentClass;

    //--------------------------------------------------------------------------
    /** Singleton Class that will create Agent objects.
        @ingroup Agent
     */
    class AgentFactory : public Singleton<AgentFactory>{
    public:
        /** Set the directory that use to lookup Lua files for Agent classes.
            @param dir directory to set.
         */
        void setDatadir( const std::string & dir );

        /** Create a new AgentClass instance.

            It will execute the Lua file that contains the AgentClass
            definitions using the name of the AgentClass as filename.

            @param name name of the AgentClass. Used as Lua filename also.
         */
        AgentClass * createClass( const std::string & name );

        /** Get an already create AgentClass, if it exists.
            @param name name of the AgentClass.
         */
        AgentClass * getClass( const std::string & name ) const;

    private:
        /// Directory where this class lookup for Lua files.
        std::string m_dir{""};
        /// List of already created AgentClass instances.
        std::map<std::string, AgentClass *> m_classes;
    };
}

//------------------------------------------------------------------------------
#endif//AGENTFACTORY_H

//------------------------------------------------------------------------------
