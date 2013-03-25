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
/** @file datastore.hpp
    @brief IO::DataStore Declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef DATASTORE_HPP
#define DATASTORE_HPP

//------------------------------------------------------------------------------
#include <map>

#include "mongo.h"
#include "singleton.hpp"
#include "agentid.hpp"

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
        /// Default Hostname of Data Store.
        constexpr static const char * const DEFAULT_HOSTNAME{"localhost"};
        /// Default Host port of Data Store.
        constexpr static uint16_t DEFAULT_PORT{27017};

        DataStore();
        ~DataStore();

        /** Save the agent instance step in the Data Store.
            @param time simulation time of the step.
            @param id identifier of Agent Instance.
            @param vars variables to save.
         */
        void saveAgentInstance( const double time, const Agent::AgentId & id, const std::map<std::string, const Util::ScriptValue *> & vars );

        /** Create a DataStore name using random UUID.
            @returns the name.
         */
        std::string mkName() const;

        /** Create a DataStore.
            @param name name of the datastore.
            @retval true if can be created.
         */
        bool createStore( const std::string & name );

        /** Set the datastore name.
            @param name name of the datastore.
         */
        void setDataStoreName( const std::string & name );
        /** Set the datastore host name.
            @param host hostname.
         */
        void setDataStoreHost( const std::string & host );

        /** Set the datastore host port.
            @param port host port.
         */
        void setDataStorePort( const uint16_t port );

    private:
        /// connect with datastore.
        /// retval true if connection was ok.
        bool connect();

        /// datastore name.
        std::string m_dbname;
        /// datastore host.
        std::string m_dbhost;
        /// datastore host port.
        uint16_t m_dbport;
        /// mongo connection attribute.
        mongo m_conn;
    };

    //--------------------------------------------------------------------------
    inline void DataStore::setDataStoreName( const std::string & name ){
        m_dbname = name;
    }

    //--------------------------------------------------------------------------
    inline void DataStore::setDataStorePort( const uint16_t port ){
        m_dbport = port;
    }

}//namespace IO

//------------------------------------------------------------------------------
#endif//DATASTORE_HPP

//------------------------------------------------------------------------------
