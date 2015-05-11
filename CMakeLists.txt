cmake_minimum_required(VERSION 3.2)
project(yasp)

add_definitions(-DDEBUG)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(yasp ${SOURCE_FILES} decode/decode.h decode/decode.c defs.h state.h stateutils.h stateutils.c decode/decode_00.h decode/decode_00.c decode/decode_10.h decode/decode_10.c)