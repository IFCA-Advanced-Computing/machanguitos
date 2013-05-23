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
/** @file agentclass.hpp
    @brief Agent::AgentClass Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef AGENTCLASS_HPP
#define AGENTCLASS_HPP

//------------------------------------------------------------------------------
#include <memory>
#include <set>
#include <string>
#include "lua.hpp"

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    /** Agent Table Name for Lua files.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_AGENT_NAME = "Agent";
    /** AgentClass Table Name for Lua files.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_AGENTCLASS_NAME = "AgentClass";

    //--------------------------------------------------------------------------
    /** Class with Class definitions of Agents.
        @ingroup Agent
     */
    class AgentClass{
    public:
        /** Constructs the AgentClass object with a initialiced Lua State.
            @param L Lua State. It should be already initializated.
         */
        AgentClass( lua_State * L );
        ~AgentClass();

        /// init class instance lua state
        void init();

        /// Returns current Lua State for this AgentClass.
        lua_State * getLua() const;
        /** Add a new variable to the list of out variables. Out variables are
            Agent instance variables that are saved to the database in each
            step.
            @param key new variable to output.
         */
        void insertOutVariable( std::string && key );
        /// Returns the list of output variables.
        const std::set<std::string> & getOutVars() const;

    private:
        /// Lua State
        lua_State * m_L;

        /// list of variables to out
        std::set< std::string > m_outVars;
    };

    //--------------------------------------------------------------------------
    inline lua_State * AgentClass::getLua() const{
        return m_L;
    }

    //--------------------------------------------------------------------------
    inline const std::set<std::string> & AgentClass::getOutVars() const {
        return m_outVars;
    }
}

//------------------------------------------------------------------------------
#endif//AGENTCLASS_HPP

//------------------------------------------------------------------------------
