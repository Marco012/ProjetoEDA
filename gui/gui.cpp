#include <gui/internal/gui.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <string>
#include "dialog/ImGuiFileDialog.h"


static GLFWwindow* g_window;


void gui_init_draw(void* window) {
	g_window = (GLFWwindow*) window;
}


void gui_open_load_file_dialog(char* key, char* title, char* filter) {
	ImGuiFileDialog::Instance()->OpenDialog(key, title, filter, ".");
	ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImVec4(1.0f, 1.0f, 1.0f, 0.5f), ICON_FA_FOLDER);
	ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "", ImVec4(0.5f, 1.0f, 0.9f, 0.5f), ICON_FA_FILE);
}


void gui_open_save_file_dialog(char* key, char* title, char* filter) {
	ImGuiFileDialog::Instance()->OpenDialog(key, title, filter, ".", "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
	ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImVec4(1.0f, 1.0f, 1.0f, 0.5f), ICON_FA_FOLDER);
	ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "", ImVec4(0.5f, 1.0f, 0.9f, 0.5f), ICON_FA_FILE);
}


bool gui_render_file_dialog(char* key, char* file_path) {
	bool done = false;

	if (ImGuiFileDialog::Instance()->Display(key)) {
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			file_path[0] = 0;
			sprintf(file_path, "%s", ImGuiFileDialog::Instance()->GetFilePathName().c_str());
			done = true;
		}

		ImGuiFileDialog::Instance()->Close();
	}

	return done;
}


void gui_draw_spacing(void) {
	ImGui::Spacing();
}


void gui_sameline() {
	ImGui::SameLine();
}


void gui_margin_x(float margin) {
	ImGui::SetCursorPosX(margin);
}


bool gui_draw_button(char* text) {
	return ImGui::Button(text);
}


void gui_get_window_pos(float* x, float* y) {
	ImVec2 pos = ImGui::GetWindowPos();
	*x = pos.x;
	*y = pos.y;
}


float gui_get_window_width() {
	return ImGui::GetWindowWidth();
}


void gui_get_cursor_pos(float* x, float* y) {
    ImVec2 cursorPos = ImGui::GetMousePos();
	*x = cursorPos.x;
	*y = cursorPos.y;
}


void gui_draw_line(float min_x, float min_y, float max_x, float max_y, float thickness, float r, float g, float b, float alpha) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();

	draw_list->AddLine(ImVec2(min_x + p.x, min_y + p.y), ImVec2(max_x + p.x, max_y + p.y), ImColor(r, g, b, alpha), thickness);
}


void gui_draw_rect_rgb(float min_x, float min_y, float max_x, float max_y, float r, float g, float b, float alpha) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();

	draw_list->AddRectFilled(ImVec2(min_x + p.x, min_y + p.y), ImVec2(max_x + p.x, max_y + p.y), ImColor(r, g, b, alpha), 6.0f);
	draw_list->AddRect(ImVec2(min_x + p.x, min_y + p.y), ImVec2(max_x + p.x, max_y + p.y), ImColor(r - 0.1f, g - 0.1f, b - 0.1f, alpha), 6.0f, 15, 3.0f);
}


void gui_draw_rect_hsv(float min_x, float min_y, float max_x, float max_y, int h, int s, int v, int alpha) {
	float r, g, b = 0;
	ImGui::ColorConvertHSVtoRGB(h / 360.0f, s / 100.0f, v / 100.0f, r, g, b);
	gui_draw_rect_rgb(min_x, min_y, max_x, max_y, r, g, b, alpha / 256.0f);
}


void gui_set_cursor_pos(float x, float y) {
	ImGui::SetCursorPos(ImVec2(x, y));
}


bool gui_draw_buttonW(char* text, float width) {
	return ImGui::Button(text, ImVec2(width - WINDOW_PADDING_TOTAL, 0));
}


bool gui_draw_button_fill(char* text) {
	float width = ImGui::GetWindowWidth();
	return ImGui::Button(text, ImVec2(-FLT_MIN, 0));
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


void gui_draw_input_uint16(const char* text, uint16_t* value) {
	ImGui::InputScalar(text, ImGuiDataType_U16, value);
}


void gui_draw_input_string(const char* text, char* value, int size) {
	ImGui::InputText(text, value, size);
}


bool gui_draw_collapsing_header(const char* text) {
	return ImGui::CollapsingHeader(text);
}


void gui_open_popup(const char* title) {
	ImGui::OpenPopup(title);
}


bool gui_begin_popup(const char* title) {
	// == true is required because it was returning random values when true.
	return ImGui::BeginPopupModal(title) == true;
}


void gui_end_popup(void) {
	ImGui::EndPopup();
}


void gui_close_popup(void) {
	ImGui::CloseCurrentPopup();
}


void gui_columns(int count) {
	ImGui::Columns(count);
}


void gui_next_column(void) {
	ImGui::NextColumn();
}


bool gui_start_page(char* title, bool *show) {
	return ImGui::Begin(title, show);
}


void gui_end_page(void) {
	ImGui::End();
}


void gui_start_menu(void) {
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