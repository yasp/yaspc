cmake_minimum_required(VERSION 3.2)
project(yasp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=C99")

set(SOURCE_FILES main.c)
add_executable(yasp ${SOURCE_FILES} decode.h decode.c defs.h)