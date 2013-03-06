include_directories(${MONGO_INCLUDE_DIRS})

add_definitions(-DMONGO_HAVE_STDINT)

add_executable(test_mongo test_mongo.cpp)
target_link_libraries(test_mongo mongoc)
