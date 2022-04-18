#ifndef GUI_VIEW
#define GUI_VIEW

#ifdef __cplusplus
extern "C" {
#endif

struct view_t;
typedef struct view_t view_t;

/**
 * @brief Registers a view with a given name.
 * @param name Name of the view.
 * @param fn_opening Function to be called when the view is opened.
 * @param fn_render Function to be called when the view is rendered.
 * @param fn_closing Function to be called whenn the view is closed.
 * @return Pointer to the view created. It might be useless.
*/
view_t* gui_register_view(const char* name, void* (*fn_opening)(view_t*, void*), void (*fn_render)(view_t*, void*, void*), void (*fn_closing)(view_t*, void*, void*));

/**
 * @brief Opens a view registered with a specific name.
 * @param name Name of the view to open.
 * @param param Optional parameter to send to the view.
*/
void gui_open_view(const char* name, void* param);

/**
 * @brief USED INTERNALLY. Render all the views.
*/
void gui_render_views();

#ifdef __cplusplus
}
#endif

#endif