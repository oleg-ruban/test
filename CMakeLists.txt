cmake_minimum_required(VERSION 3.27)
project(lab1 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab1 lab1.c)

target_link_libraries(lab1 m)
