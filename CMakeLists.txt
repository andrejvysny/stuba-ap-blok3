cmake_minimum_required(VERSION 3.21)
project(Blok3 C)

set(CMAKE_C_STANDARD 99)

link_libraries(ws2_32 wsock32)

add_executable(Blok3 main.c)

