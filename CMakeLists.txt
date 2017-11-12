cmake_minimum_required(VERSION 3.8)
project(lab5)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c functions.c sepia.c code.asm)
add_executable(lab5 ${SOURCE_FILES})