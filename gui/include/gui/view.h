#ifndef GUI_VIEW
#define GUI_VIEW

#ifdef __cplusplus
extern "C" {
#endif

struct view_t;
typedef struct view_t view_t;

view_t* gui_register_view(const char* name, void* (*fn_opening)(view_t*, void*), void (*fn_render)(view_t*, void*, void*), void (*fn_closing)(view_t*, void*, void*));

void gui_open_view(const char* name, void* param);

void gui_render_views();

#ifdef __cplusplus
}
#endif

#endif