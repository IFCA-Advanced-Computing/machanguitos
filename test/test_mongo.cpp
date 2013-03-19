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
#include <iostream>
#include <cstdlib>
#include "mongo.h"

constexpr const char * TEST_SERVER = "localhost";
constexpr int TEST_PORT = 27017;

//------------------------------------------------------------------------------
int main( int argc, char * argv[] ){
    mongo conn;

    if( mongo_client( &conn , TEST_SERVER, TEST_PORT ) != MONGO_OK ) {
        std::cout << "failed to connect\n";
        return EXIT_FAILURE;
    }

    mongo_cursor cursor;
    mongo_cursor_init( &cursor, &conn, "test.test" );

    char hex_oid[25];
    while( mongo_cursor_next( &cursor ) == MONGO_OK ) {
        std::cout << "row:\n";
        bson_iterator it;
        bson_iterator_init( &it, mongo_cursor_bson( &cursor ) );
        while( bson_iterator_next( &it ) ) { 
            std::cout << "  " << bson_iterator_key( &it ) << " = ";
            switch( bson_iterator_type( &it ) ) {
            case BSON_DOUBLE:
                std::cout << "(double) " << bson_iterator_double( &it ) << std::endl;
                break;
            case BSON_INT:
                std::cout << "(int) " << bson_iterator_int( &it ) << std::endl;
                break;
            case BSON_STRING:
                std::cout << "(string) \"" << bson_iterator_string( &it ) << "\"\n";
                break;
            case BSON_OID:
                bson_oid_to_string( bson_iterator_oid( &it ), hex_oid );
                std::cout << "(oid) \"" << hex_oid << "\"\n";
                break;
            case BSON_OBJECT:
                std::cout << "(subobject) {...}\n";
                break;
            case BSON_ARRAY:
                std::cout << "(array) [...]\n";
                break;
            case BSON_TIMESTAMP:
                std::cout << "(timestamp) [...]\n";
                break;
            default:
                std::cout << "(type " << bson_iterator_type( &it ) << std::endl;
                break;
            }
        }
        std::cout << std::endl;
    }


    mongo_disconnect( &conn );

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
