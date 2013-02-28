file(GLOB MACHANGUITOS_SOURCES "*.cpp")

include_directories(${LUA_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})

if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
endif()

link_directories(${Boost_LIBRARY_DIRS})

if(MPI_FOUND)
  set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
  set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
endif()

add_executable(machen ${MACHANGUITOS_SOURCES})

target_link_libraries(machen ${LUA_LIBRARIES})
target_link_libraries(machen ${Boost_LIBRARIES})
if(MPI_FOUND)
  target_link_libraries(machen ${MPI_LIBRARIES})
endif()

install(TARGETS machen RUNTIME DESTINATION bin)
