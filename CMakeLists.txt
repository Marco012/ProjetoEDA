cmake_minimum_required(VERSION 3.0.0)
project(ProjetoEDA VERSION 0.1.0)

include_directories(ProjetoEDA PRIVATE include)

FILE(GLOB srcs src/*.c)
add_executable(ProjetoEDA ${srcs})