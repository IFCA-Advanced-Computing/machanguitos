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
/** @file agentid.hpp
    @brief Agent::AgentID declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef AGENTID_HPP
#define AGENTID_HPP

//------------------------------------------------------------------------------
#include <string>

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    /** Identifier of an AgentInstance.
        @ingroup Agent
     */
    class AgentId {
    public:
        /** AgentId constructor.
            @param mayor mayor number of AgentId.
            @param minor minor number of AgentId.
         */
        AgentId( const int mayor, const int minor );

        /// Returns the string representation of ID.
        std::string str() const;
        /** Returns the safe string representation of id. The safe
            representation also is valid as a name in databases. */
        std::string idstr() const;

        /// mayor component of identifier.
        int m_mayor;
        /// minor component of identifier.
        int m_minor;
    };

    //--------------------------------------------------------------------------
    inline AgentId::AgentId( const int my, const int mn )
        : m_mayor(my), m_minor(mn)
    {
        // empty
    }

    //--------------------------------------------------------------------------
    inline std::string AgentId::str() const{
        return std::to_string(m_mayor) + "." + std::to_string(m_minor);
    }

    //--------------------------------------------------------------------------
    inline std::string AgentId::idstr() const{
        return "m" + std::to_string(m_mayor) + "n" + std::to_string(m_minor);
    }

}

//------------------------------------------------------------------------------
#endif//AGENTID_HPP

//------------------------------------------------------------------------------
