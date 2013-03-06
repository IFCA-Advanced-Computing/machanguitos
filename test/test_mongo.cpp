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
