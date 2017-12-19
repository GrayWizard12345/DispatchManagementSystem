cmake_minimum_required(VERSION 3.8)
project(DispatchManagementSystem)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/DMSystem_starter.c Structures/Location.h Structures/Order.h Structures/Driver.h Structures/Connection.h Structures/Client.h Structures/Server.h View/ClientView.h View/DriverView.h JSON/JSON_parser.h JSON/cJSON.h JSON/cJSON.c src/driver/driver_main.c global_var/global_var.h global_var/enums.h JSON/JSON_encoder.h)
add_executable(DispatchManagementSystem ${SOURCE_FILES} JSON/cJSON.h JSON/cJSON.c)
add_executable(driver src/driver/driver_main.c)