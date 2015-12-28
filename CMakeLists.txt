project(MACHANGUITOS)
cmake_minimum_required(VERSION 2.6)
set(MACHANGUITOS_VERSION_MAJOR 0)
set(MACHANGUITOS_VERSION_MINOR 7)
set(MACHANGUITOS_VERSION_RELEASE 1)

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
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -fno-rtti" )
  else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x -fno-rtti" )
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "-pedantic -g -O0" )
  set(CMAKE_CXX_FLAGS_RELEASE "-O3" )
  set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg" )

  set(CMAKE_EXEC_LINKER_FLAGS_PROFILE "-pg" )
else()
#  message("compiler detected as: non-GNU compiler")
endif()

find_package(LuaJIT REQUIRED)
set(LUA_INCLUDE_DIRS ${LuaJIT_INCLUDE_DIRS})
set(LUA_LIBRARIES ${LuaJIT_LIBRARIES})

set(Boost_USE_MULTITHREADED 0)
find_package( Boost 1.52 COMPONENTS system filesystem REQUIRED )

find_package( MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

find_package( GDAL REQUIRED )

include(CheckIncludeFile)
check_include_file("getopt.h" HAVE_GETOPT_H)
if(NOT HAVE_GETOPT_H)
  message( FATAL_ERROR "getopt Not Found" )
endif()

set(DATADIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}-${MACHANGUITOS_VERSION_MAJOR}.${MACHANGUITOS_VERSION_MINOR}")

if(CMAKE_HOST_UNIX)
  execute_process( COMMAND getconf PATH_MAX /
    OUTPUT_VARIABLE OBJECT_PATH_MAX OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process( COMMAND getconf NAME_MAX /
    OUTPUT_VARIABLE OBJECT_NAME_MAX OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
  set(OBJECT_PATH_MAX 260)
  set(OBJECT_NAME_MAX 255)
endif()

message( "OBJECT ${CMAKE_HOST_SYSTEM_NAME} ${OBJECT_PATH_MAX} ${OBJECT_NAME_MAX}" )
configure_file ("${PROJECT_SOURCE_DIR}/config.h.in"
                "${PROJECT_BINARY_DIR}/config.h" )

set(MONGO_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/mongoc")

add_subdirectory(mongoc)
add_subdirectory(common)
add_subdirectory(machen)
add_subdirectory(outcsv)
add_subdirectory(test)

#data files
install(DIRECTORY "example" DESTINATION ${DATADIR})
install(DIRECTORY "data" DESTINATION ${DATADIR})

# documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_INPUTS
    "${PROJECT_SOURCE_DIR}/doc/doxydoc ${PROJECT_SOURCE_DIR}/mongoc ${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/machen ${PROJECT_SOURCE_DIR}/outcsv")
  set(DOXYGEN_DEFINES "")
  if(MPI_FOUND)
    set(DOXYGEN_DEFINES "${DOXYGEN_DEFINES} HAVE_MPI")
  endif()

  configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
