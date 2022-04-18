#ifndef GUI_WINDOW
#define GUI_WINDOW


#define WINDOW_PADDING 8
#define WINDOW_PADDING_TOTAL (WINDOW_PADDING * 2)

#ifdef __cplusplus
extern "C" {
#endif

struct window_t;
typedef struct window_t window_t;


/**
 * @brief Inits the window.
 * @return 1 if succeeded, 0 if an error occurred.
*/
int window_init();

/**
 * @brief Creates a window instance.
 * @return Window created. It can be null.
*/
window_t* window_create();

/**
 * @brief Renders a window.
 * @param window Window to render.
 * @param fn_draw Function to be called to draw extra stuff.
 * @return 0 when the window is closed, if the window is opened returns 1.
*/
int window_render(window_t* window, void (*fn_draw) (window_t*));

/**
 * @brief Close a window.
 * @param window Window to close.
*/
void window_close(window_t* window);

/**
 * @brief Clears all the memory allocated by the window.
 * @param window Window to destroy.
*/
void window_destroy(window_t* window);

#ifdef __cplusplus
}
#endif

#endif