file(GLOB MACHEN_SOURCES "*.cpp")

include_directories(${LUA_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${MONGO_INCLUDE_DIRS})
include_directories(${MPI_INCLUDE_PATH})
include_directories(${GDAL_INCLUDE_DIR})

add_definitions(-DMONGO_HAVE_STDINT)

link_directories(${Boost_LIBRARY_DIRS})

add_executable(machen ${MACHEN_SOURCES})

target_link_libraries(machen ${LUA_LIBRARIES})
target_link_libraries(machen ${Boost_LIBRARIES})
target_link_libraries(machen ${GDAL_LIBRARY})
target_link_libraries(machen mongoc common)
if(MPI_FOUND)
  target_link_libraries(machen ${MPI_LIBRARIES})
endif()

install(TARGETS machen RUNTIME DESTINATION bin)
