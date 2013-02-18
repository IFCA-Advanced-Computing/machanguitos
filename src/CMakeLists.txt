include_directories(${LUA_INCLUDE_DIRS})
include_directories(${MACHANGUITOS_SOURCE_DIR}/src)

add_executable(machan main)

target_link_libraries(machan ${LUA_LIBRARIES})
