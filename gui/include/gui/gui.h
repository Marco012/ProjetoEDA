#ifndef GUI_GUI
#define GUI_GUI

#include "window.h"
#include "view.h"
#include "font_icons.h"
#include <inttypes.h>


/**
 * @brief USED INTERNALLY.
 * @param window GLFW window to be set as the global window used by the GUI stuff.
*/
void gui_init_draw(void* window);


#ifdef __cplusplus
extern "C" {
#endif

// Bascially, all the functions bellow are a wrapper to the ImGui API.

void gui_open_load_file_dialog(char* key, char* title, char* filter);
void gui_open_save_file_dialog(char* key, char* title, char* filter);
bool gui_render_file_dialog(char* key, char* file_path);

float gui_get_window_width();

void gui_get_window_pos(float* x, float* y);
void gui_get_cursor_pos(float* x, float* y);

void gui_draw_rect_rgb(float min_x, float min_y, float max_x, float max_y, float r, float g, float b, float alpha);
void gui_draw_rect_hsv(float min_x, float min_y, float max_x, float max_y, int h, int s, int v, int alpha);
void gui_draw_line(float min_x, float min_y, float max_x, float max_y, float thickness, float r, float g, float b, float alpha);

void gui_set_cursor_pos(float x, float y);

void gui_start_menu(void);
void gui_end_menu(float width);

void gui_draw_spacing(void);
void gui_sameline();
void gui_margin_x(float margin);

bool gui_draw_button(char* text);
bool gui_draw_buttonW(char* text, float width);
bool gui_draw_button_fill(char* text);

void gui_draw_text(const char* fmt, ...);
void gui_draw_text_centered(const char* text);

void gui_draw_title(const char* text);
void gui_draw_title_centered(const char* text);

void gui_draw_input_uint16(const char* text, uint16_t* value);
void gui_draw_input_string(const char* text, char* value, int size);

bool gui_draw_collapsing_header(const char* text);

void gui_open_popup(const char* title);
bool gui_begin_popup(const char* title);
void gui_end_popup(void);
void gui_close_popup(void);

void gui_columns(int count);
void gui_next_column(void);

bool gui_start_page(char* title, bool* show);
void gui_end_page(void);


#ifdef __cplusplus
}
#endif

#endif