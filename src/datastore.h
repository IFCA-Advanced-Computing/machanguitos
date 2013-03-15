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
/** @file datastore.h
    @brief IO::DataStore Declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef DATASTORE_H
#define DATASTORE_H

//------------------------------------------------------------------------------
#include <map>

#include "singleton.h"
#include "agentinstance.h"

//------------------------------------------------------------------------------
namespace Util {
    class ScriptValue;
}

//------------------------------------------------------------------------------
namespace IO {
    //--------------------------------------------------------------------------
    /** Singleton Class that servers as proxy of Data Store.
        @ingroup IO
     */
    class DataStore : public Singleton<DataStore> {
    public:
        DataStore();

        /** Save the agent instance step in the Data Store.
            @param time simulation time of the step.
            @param id identifier of Agent Instance.
            @param vars variables to save.
         */
        void saveAgentInstance( const double time, const Agent::AgentId & id, const std::map<std::string, const Util::ScriptValue *> & vars );

    private:
        std::string m_dbname;
        std::string m_dbhost;
        int m_dbport;
    };

}//namespace IO

//------------------------------------------------------------------------------
#endif//DATASTORE_H

//------------------------------------------------------------------------------
