#ifndef GUI_GUI
#define GUI_GUI

#include "window.h"
#include "view.h"
#include "font_icons.h"


void gui_init_draw(void* window);


#ifdef __cplusplus
extern "C" {
#endif


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