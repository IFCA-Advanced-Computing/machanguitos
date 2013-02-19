file(GLOB MACHANGUITOS_SOURCES "*.cpp")

include_directories(${LUA_INCLUDE_DIRS})
include_directories(${MACHANGUITOS_SOURCE_DIR}/src)

add_executable(machen ${MACHANGUITOS_SOURCES})

target_link_libraries(machen ${LUA_LIBRARIES})
