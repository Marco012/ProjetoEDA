#ifndef GUI_GUI
#define GUI_GUI

#include"window.h"


void gui_init_draw(void* window);


#ifdef __cplusplus
extern "C" {
#endif


void gui_start_menu();
void gui_end_menu(float width);

void gui_draw_spacing();
bool gui_draw_button(char* text, float width);

void gui_draw_text(const char* fmt, ...);
void gui_draw_text_centered(const char* text);

void gui_draw_title(const char* text);
void gui_draw_title_centered(const char* text);

#ifdef __cplusplus
}
#endif

#endif