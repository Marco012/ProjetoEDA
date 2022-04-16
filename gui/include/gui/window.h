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
 * Inits the window. Returns 1 if succeeded, 0 if an error occurred.
 */
int window_init();

/**
 *  Creates a window instance.
 */
window_t* window_create();

/**
 * Renders a window and handles associated events. 
 * Returns 0 when the window is closed.
 */
int window_render(window_t* window, void (*fn_draw) (window_t*));


void window_close(window_t* window);


/**
 * Clears all the memory allocated by the window.
 */
void window_destroy(window_t* window);

#ifdef __cplusplus
}
#endif

#endif