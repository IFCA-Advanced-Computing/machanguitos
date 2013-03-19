file(GLOB COMMON_SOURCES "*.cpp")

include_directories(${LUA_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${MONGO_INCLUDE_DIRS})

add_definitions(-DMONGO_HAVE_STDINT)

if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
endif()

add_library(common STATIC ${COMMON_SOURCES})
target_link_libraries(common mongoc)
