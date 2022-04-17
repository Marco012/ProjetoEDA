#ifndef GUI_GUI
#define GUI_GUI

#include "window.h"
#include "view.h"
#include "font_icons.h"


void gui_init_draw(void* window);


#ifdef __cplusplus
extern "C" {
#endif


void gui_start_menu();
void gui_end_menu(float width);

void gui_draw_spacing();
bool gui_draw_button(char* text, float width);
bool gui_draw_button_fill(char* text);

void gui_draw_text(const char* fmt, ...);
void gui_draw_text_centered(const char* text);

void gui_draw_title(const char* text);
void gui_draw_title_centered(const char* text);

bool gui_start_page(char* title, bool* show);
void gui_end_page();

#ifdef __cplusplus
}
#endif

#endif