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
//------------------------------------------------------------------------------
#ifndef AGENTCLASS_H
#define AGENTCLASS_H

//------------------------------------------------------------------------------
#include "lua.hpp"
#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    /** Agent Table Name for Lua files.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_AGENT_NAME = "Agent";

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

        /// Create a AgentInstance object of this AgentClass.
        AgentInstance * createInstance();
        /// Returns current Lua State for this AgentClass.
        lua_State * getLua() const;

    private:
        /// Lua State
        lua_State * m_L;
    };

    //--------------------------------------------------------------------------
    inline lua_State * AgentClass::getLua() const{
        return m_L;
    }

    //--------------------------------------------------------------------------
    inline AgentInstance * AgentClass::createInstance(){
        return (new (std::nothrow) AgentInstance(this));
    }
}

//------------------------------------------------------------------------------
#endif//AGENTCLASS_H

//------------------------------------------------------------------------------
