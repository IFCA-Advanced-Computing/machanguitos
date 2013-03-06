file(GLOB MONGOC_SOURCES "*.c")

include(TestBigEndian)
test_big_endian(MONGO_BIG_ENDIAN)
if(MONGO_BIG_ENDIAN)
  add_definitions(-DMONGO_BIG_ENDIAN)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c99 -Wall)
else()
#  message("compiler detected as: non-GNU compiler")
endif()

add_definitions(-D_POSIX_SOURCE)

add_library(mongoc STATIC ${MONGOC_SOURCES})
