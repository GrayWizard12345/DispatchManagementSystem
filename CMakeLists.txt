cmake_minimum_required(VERSION 3.8)
project(DispatchManagementSystem)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/DMSystem_starter.c Structures/Location.h)
add_executable(DispatchManagementSystem ${SOURCE_FILES})