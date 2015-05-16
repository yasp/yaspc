cmake_minimum_required(VERSION 3.2)
project(yasp)

set(SOURCE_FILES main.c decode/decode.h decode/decode.c defs.h state.h stateutils.h stateutils.c decode/decode_00.h decode/decode_00.c decode/decode_10.h decode/decode_10.c controlsocket.c controlsocket.h controlcommands.h controlcommands.c runner.h runner.c )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Werror -Wno-unused-parameter")
set(CMAKE_BUILD_TYPE Debug)

add_executable(yasp ${SOURCE_FILES})
add_executable(yaspdbg ${SOURCE_FILES})

set_target_properties (yaspdbg PROPERTIES COMPILE_DEFINITIONS "DEBUG")
