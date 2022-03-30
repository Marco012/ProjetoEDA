cmake_minimum_required(VERSION 3.0)
project(ProjetoEDA C)

set(CMAKE_C_STANDARD 99)
include_directories(ProjetoEDA PRIVATE include)

FILE(GLOB srcs src/*.c)
add_executable(ProjetoEDA ${srcs})