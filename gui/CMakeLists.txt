cmake_minimum_required (VERSION 3.8)

project ("ProjectGUI")
set(CMAKE_CXX_STANDARD 17)

set(LIBS glfw glew_s imgui)

add_subdirectory (libs)

include_directories("libs/glfw/include")
include_directories("libs/glew/include")
include_directories("libs/imgui")
include_directories("include")

file(GLOB_RECURSE imgui_src CONFIGURE_DEPENDS imgui/*.cpp imgui/*.h)

add_library(${PROJECT_NAME} "window.cpp" "gui.cpp" "font.cpp" "font.h" "include/gui/window.h" ${imgui_src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})