/*******************************************************************************
Machanguitos is The Easiest Multi-Agent System in the world. Work done at The
Institute of Physics of Cantabria (IFCA).
Copyright (C) 2013  Luis Cabellos

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
/** @file agentfactory.hpp
    @brief Agent::AgentFactory Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef AGENTFACTORY_HPP
#define AGENTFACTORY_HPP

//------------------------------------------------------------------------------
#include "singleton.hpp"

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
#endif//AGENTFACTORY_HPP

//------------------------------------------------------------------------------
