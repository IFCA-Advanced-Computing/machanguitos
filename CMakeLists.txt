project(MACHANGUITOS)
cmake_minimum_required(VERSION 2.6)
set(MACHANGUITOS_VERSION_MAJOR 0)
set(MACHANGUITOS_VERSION_MINOR 5)
set(MACHANGUITOS_VERSION_RELEASE 0)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DDEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
  add_definitions(-DRELEASE)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE CXX_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${CXX_VERSION} VERSION_GREATER "4.6.99")
    set(CMAKE_CXX_FLAGS "-Wall -std=c++11" )
  else()
    set(CMAKE_CXX_FLAGS "-Wall -std=c++0x" )
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "-pedantic -g -O0" )
  set(CMAKE_CXX_FLAGS_RELEASE "-O3" )
else()
#  message("compiler detected as: non-GNU compiler")
endif()

find_package(LuaJIT REQUIRED)
set(LUA_INCLUDE_DIRS ${LuaJIT_INCLUDE_DIRS})
set(LUA_LIBRARIES ${LuaJIT_LIBRARIES})

set(Boost_USE_MULTITHREADED 0)
find_package( Boost 1.52 COMPONENTS system filesystem REQUIRED )

find_package( MPI )
if(MPI_FOUND)
  add_definitions(-DHAVE_MPI)
endif()

set(DATADIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}-${MACHANGUITOS_VERSION_MAJOR}.${MACHANGUITOS_VERSION_MINOR}")

configure_file ("${PROJECT_SOURCE_DIR}/config.h.in"
                "${PROJECT_BINARY_DIR}/config.h" )

add_subdirectory(src)

#data files
install(DIRECTORY "example" DESTINATION ${DATADIR})

# documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
