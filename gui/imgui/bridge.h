#include <imgui/imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


void imgui_init(GLFWwindow* window, const char* glsl_version);

void imgui_prerender();

void imgui_render();