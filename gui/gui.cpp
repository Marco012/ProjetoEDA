#include <gui/internal/gui.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <string>


static GLFWwindow* g_window;


void gui_init_draw(void* window) {
	g_window = (GLFWwindow*) window;
}


void gui_draw_spacing() {
	ImGui::Spacing();
}


bool gui_draw_button(char* text, float width) {
	return ImGui::Button(text, ImVec2(width - WINDOW_PADDING_TOTAL, 30));
}


bool gui_draw_button_fill(char* text) {
	float width = ImGui::GetWindowWidth();
	return gui_draw_button(text, width);
}


void gui_draw_text(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ImGui::TextV(fmt, args);
	va_end(args);
}


void gui_draw_text_centered(const char* text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text);
}


void gui_draw_title(const char* text) {
	ImGui::PushFont(title_font);
	gui_draw_text(text);
	ImGui::PopFont();
}


void gui_draw_title_centered(const char* text) {
	ImGui::PushFont(title_font);
	gui_draw_text_centered(text);
	ImGui::PopFont();
}


bool gui_start_page(char* title, bool *show) {
	return ImGui::Begin(title, show);
}


void gui_end_page() {
	ImGui::End();
}


void gui_start_menu() {
	ImGui::Begin("menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(ImVec2(0, 0));
}


void gui_end_menu(float width) {

	int display_w, display_h;
	glfwGetFramebufferSize(g_window, &display_w, &display_h);

	float height = ImGui::GetWindowHeight();
	ImGui::SetWindowPos(ImVec2(display_w / 2 - width / 2, display_h / 2 - height / 2));

	ImGui::SetWindowSize(ImVec2(width, 0));

	ImGui::End();
}