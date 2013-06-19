include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${MONGO_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GDAL_INCLUDE_DIR})

add_definitions(-DMONGO_HAVE_STDINT)

add_executable(test_mongo test_mongo.cpp)
target_link_libraries(test_mongo mongoc)

add_executable(test_uuid test_uuid.cpp)

add_executable(test_cpp11 test_cpp11.cpp)

add_executable(test_gdal test_gdal.cpp)
target_link_libraries(test_gdal ${GDAL_LIBRARY})
target_link_libraries(test_gdal common)
target_link_libraries(test_gdal ${LUA_LIBRARIES})
target_link_libraries(test_gdal ${Boost_LIBRARIES})
