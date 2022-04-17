#include <gui/gui.h>
#include <imgui/imgui.h>
#include <string>
#include <map>


#define VIEW_STATE_CLOSED	0
#define VIEW_STATE_OPENING	1
#define VIEW_STATE_OPENED	2
#define VIEW_STATE_CLOSING	3


struct view_t {
	void* (*fn_opening)(view_t*);
	void (*fn_render)(view_t*, void*);
	void (*fn_closing)(view_t*, void*);
	std::string name;
	void* data;
	uint8_t state;
};


std::map<std::string, view_t*> views;


view_t* gui_register_view(const char* name, void* (*fn_opening)(view_t*), void (*fn_render)(view_t*, void*), void (*fn_closing)(view_t*, void*)) {
	view_t* view = new view_t;
	
	view->name.assign(name);
	view->data = NULL;
	view->state = VIEW_STATE_CLOSED;
	view->fn_opening = fn_opening;
	view->fn_render = fn_render;
	view->fn_closing = fn_closing;

	views.insert(std::pair<std::string, view_t*>(view->name, view));

	return view;
}


void gui_open_view(const char* name) {
	std::string str_name(name);

	auto view = views.find(str_name);
	if (view != views.end())
		view->second->state = VIEW_STATE_OPENING;
}


void gui_render_views() {
	for (auto it = views.begin(); it != views.end(); it++)
	{
		view_t* view = it->second;
		bool is_showing = true;

		if (view->state == VIEW_STATE_OPENING) {
			if (view->fn_opening)
				view->data = view->fn_opening(view);

			view->state = VIEW_STATE_OPENED;
		}

		if (view->state == VIEW_STATE_OPENED) {
			ImGui::Begin(view->name.c_str(), &is_showing);

			if (view->fn_render)
				view->fn_render(view, view->data);

			ImGui::End();

			if (!is_showing)
				view->state = VIEW_STATE_CLOSING;
		}
		
		if (view->state == VIEW_STATE_CLOSING) {
			if (view->fn_closing)
				view->fn_closing(view, view->data);

			view->state = VIEW_STATE_CLOSED;	
		}
	}
}