cmake_minimum_required(VERSION 3.17)
project(Server_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Server_Project main.c cJSON.c)
target_link_libraries(Server_Project ws2_32)