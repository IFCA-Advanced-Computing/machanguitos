include(CheckFunctionExists)
check_function_exists("mkstemp" EXISTS_MKSTEMP)
check_function_exists("gmtime_r" EXISTS_GMTIME_R)

if(${EXISTS_MKSTEMP})
  add_definitions(-DLUA_USE_MKSTEMP)
endif()

if(${EXISTS_GMTIME_R})
  add_definitions(-DLUA_USE_GMTIME_R)
endif()

# define the lua core source files
set(LUA_CORE_SRCS lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c
  lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c
  lstate.c lstring.c ltable.c ltm.c  lundump.c lvm.c lzio.c)

# define the lua lib source files
set(LUA_LIB_SRCS lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c
  liolib.c lmathlib.c loslib.c ltablib.c lstrlib.c loadlib.c linit.c)

# create the library
add_library (lua ${LUA_LIB_SRCS} ${LUA_CORE_SRCS})
