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
#include <string>
#include <map>
#include "common/singleton.hpp"
#include "agentinstance.hpp"

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
        /** Create a new AgentClass instance.

            It will execute the Lua file that contains the AgentClass
            definitions using the name of the AgentClass as filename.

            @param name name of the AgentClass. Used as Lua filename also.
         */
        std::shared_ptr<AgentClass> createClass( const std::string & name );

        /** Get an already create AgentClass, if it exists.
            @param name name of the AgentClass.
         */
        std::shared_ptr<AgentClass> getClass( const std::string & name ) const;

        /// Create a AgentInstance object of this AgentClass.
        std::unique_ptr<AgentInstance> createInstance( std::shared_ptr<AgentClass> aclass, AgentId && id );
    private:
        /// List of already created AgentClass instances.
        std::map<std::string, std::shared_ptr<AgentClass>> m_classes;
    };

    //--------------------------------------------------------------------------
    inline std::unique_ptr<AgentInstance> AgentFactory::createInstance(
          std::shared_ptr<AgentClass> aclass, AgentId && id )
    {
        return std::unique_ptr<AgentInstance>( new (std::nothrow) AgentInstance( aclass, std::move(id) ));
    }

}

//------------------------------------------------------------------------------
#endif//AGENTFACTORY_HPP

//------------------------------------------------------------------------------
