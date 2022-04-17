cmake_minimum_required (VERSION 3.8)

project ("ProjetoEDA")
set(CMAKE_C_STANDARD 99)

add_subdirectory(gui)
include_directories(${PROJECT_NAME} "include" "gui/include")

FILE(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.c)
add_executable(${PROJECT_NAME} ${srcs})

target_link_libraries(${PROJECT_NAME} PRIVATE ProjectGUI)
