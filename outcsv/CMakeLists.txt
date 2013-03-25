file(GLOB OUTCSV_SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${MONGO_INCLUDE_DIRS})

add_definitions(-DMONGO_HAVE_STDINT)

link_directories(${Boost_LIBRARY_DIRS})

add_executable(machen-outcsv ${OUTCSV_SOURCES})

target_link_libraries(machen-outcsv ${Boost_LIBRARIES})
target_link_libraries(machen-outcsv mongoc common)

install(TARGETS machen-outcsv RUNTIME DESTINATION bin)
