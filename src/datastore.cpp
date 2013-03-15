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
#include "datastore.h"

#include <iostream>
#include <string>
#include "mongo.h"

//------------------------------------------------------------------------------
namespace IO {
    using namespace std;
    using namespace Agent;
    using namespace Util;

    //--------------------------------------------------------------------------
    constexpr const char * TEST_SERVER = "localhost";
    constexpr int TEST_PORT = 27017;

    //--------------------------------------------------------------------------
    void DataStore::saveAgentInstance( const double time, const AgentId & id,
                                       const map<string, const ScriptValue *> & vars )
    {
        if( vars.size() == 0 ){
            return;
        }

        mongo conn;

        if( mongo_client( &conn , TEST_SERVER, TEST_PORT ) != MONGO_OK ) {
            cout << "failed to connect mongo\n";
            return;
        }

        string ns{ "test.instance." + id.str() };

        bson b;

        bson_init( &b );
        if( bson_append_new_oid( &b, "_id" ) != BSON_OK ){
            cout << "failed to create bson id\n";
            mongo_disconnect( &conn );
            return;
        }

        bson_append_double( &b, "time", time );

        for( const auto i: vars ){
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

        if( mongo_insert( &conn , ns.c_str() , &b, NULL ) != MONGO_OK ){
            cout << "failed to insert in mongo\n";
        }

        bson_destroy( &b );

        mongo_disconnect( &conn );
    }

}//namespace IO

//------------------------------------------------------------------------------
