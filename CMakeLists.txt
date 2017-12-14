cmake_minimum_required(VERSION 3.8)
project(DispatchManagementSystem)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/DMSystem_starter.c Structures/Location.h Structures/Order.h Structures/Driver.h Structures/Connection.h Structures/Client.h Structures/Server.h View/ClientView.h View/DriverView.h)
add_executable(DispatchManagementSystem ${SOURCE_FILES})