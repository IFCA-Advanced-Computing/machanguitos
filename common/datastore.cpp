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
/** @file datastore.cpp
    @brief IO::DataStore Definition.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "datastore.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "config.h"
#include "scriptvalue.hpp"
#include "log.hpp"

//------------------------------------------------------------------------------
namespace IO {
    using namespace std;
    using namespace Agent;
    using namespace Util;
    using namespace boost;

    //--------------------------------------------------------------------------
    /** private prefix of agent collections in mongo.
        @ingroup IO
     */
    constexpr const char * AGENT_COLL_PREFIX = ".ins.";

    //--------------------------------------------------------------------------
    DataStore::DataStore()
        : m_dbname{"_ds"}, m_dbhost{DEFAULT_HOSTNAME}, m_dbport{DEFAULT_PORT},
        m_isConnected{false}
    {
        mongo_init( &m_conn );
    }

    //--------------------------------------------------------------------------
    DataStore::~DataStore(){
        mongo_destroy( &m_conn );
    }

    //--------------------------------------------------------------------------
    void DataStore::setDataStoreHost( const string & host ){
        if( host.length() < MAX_HOST_NAME ){
            m_dbhost = host;
        }else{
            LOGW( "Datastore host too long '", host, "'" );
        }
    }

    //--------------------------------------------------------------------------
    string DataStore::mkName() const{
        uuids::random_generator gen;
        uuids::uuid u = gen();

        ostringstream strval;
        for( const auto& i: u ){
            strval.fill('0');
            strval.width(2);
            strval << uppercase << hex << static_cast<unsigned>(i);
        }

        return "d" + strval.str();
    }

    //--------------------------------------------------------------------------
    bool DataStore::connect() {
        if( !m_isConnected ){
            m_isConnected = mongo_client( &m_conn , m_dbhost.c_str(), m_dbport )
                == MONGO_OK;
            if( !m_isConnected ) {
                LOGE( "failed to connect ", m_dbhost, ":", m_dbport );
                LOGE( "  mongo error: ", m_conn.err );
            }
        }
        return m_isConnected;
    }

    //--------------------------------------------------------------------------
    void DataStore::disconnect() {
        if( m_isConnected ){
            mongo_disconnect( &m_conn );
            m_isConnected = false;
        }
    }

    //--------------------------------------------------------------------------
    bool DataStore::createStore( const string & name ){
        m_dbname = name;

        if( !connect() ) {
            return false;
        }

        if( mongo_cmd_drop_db( &m_conn, name.c_str() ) != MONGO_OK ){
            LOGE( "failed to drop database ", name );
            return false;
        }

        disconnect();

        return true;
    }

    //--------------------------------------------------------------------------
    void DataStore::saveAgentInstance( const double time, const AgentId & id,
                                       const map<string, const ScriptValue *> & vars )
    {
        if( vars.size() == 0 || !m_isConnected ){
            return;
        }

        string ns{ m_dbname + AGENT_COLL_PREFIX + "m" + to_string(id.m_mayor) };

        bson b;

        bson_init( &b );
        if( bson_append_new_oid( &b, "_id" ) != BSON_OK ){
            LOGE( "failed to create bson id" );
            mongo_disconnect( &m_conn );
            return;
        }

        bson_append_int( &b, "minor", id.m_minor );
        bson_append_double( &b, "time", time );

        for( const auto& i: vars ){
            const ScriptValue & val = *(i.second);
            switch( val.getType() ){
            case ScriptValue::ValueType::NIL: // empty
                break;
            case ScriptValue::ValueType::BOOLEAN:
                bson_append_bool( &b, i.first.c_str(), val.getBoolean() );
                break;
            case ScriptValue::ValueType::NUMBER:
                bson_append_double( &b, i.first.c_str(), val.getNumber() );
                break;
            case ScriptValue::ValueType::STRING:
                bson_append_string( &b, i.first.c_str(), val.getString().c_str() );
                break;
            }
        }

        bson_finish( &b );

        if( mongo_insert( &m_conn , ns.c_str() , &b, NULL ) != MONGO_OK ){
            LOGE( "failed to insert in mongo" );
        }

        bson_destroy( &b );
    }

}//namespace IO

//------------------------------------------------------------------------------
