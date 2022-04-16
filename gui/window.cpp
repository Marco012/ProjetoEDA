#include <gui/internal/gui.h>
#include "font.h"


#define BACKGROUND_COLOR_R 0.1f
#define BACKGROUND_COLOR_G 0.1f
#define BACKGROUND_COLOR_B 0.1f


ImFont* title_font;


struct window_t {
	GLFWwindow* window;
};


#if __APPLE__
const char* glsl_version = "#version 150";
#else
const char* glsl_version = "#version 130";
#endif


static void window_apply_imgui_style(GLFWwindow* window)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4 darkColor = ImVec4(0.14f, 0.14f, 0.14f, 1);
	ImVec4 mediumColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
	ImVec4 mediumColorHover = ImVec4(mediumColor.x + 0.04f, mediumColor.y + 0.04f, mediumColor.z + 0.04f, 1.00f);
	ImVec4 lightColor = ImVec4(0.3f, 0.3f, 0.3f, 1);
	ImVec4 lightColorHover = ImVec4(lightColor.x - 0.04f, lightColor.y - 0.04f, lightColor.z - 0.04f, 1.00f);
	ImVec4 textColor = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
	ImVec4 textColorDisabled = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	ImVec4 primaryColor = ImVec4(165 / 255.0f, 85 / 255.0f, 33 / 255.0f, 1.00f);
	ImVec4 primaryColorHover = ImVec4(primaryColor.x, primaryColor.y, primaryColor.z, 0.8f);

	ImFontConfig defaultConfig;
	defaultConfig.FontDataOwnedByAtlas = false;
	auto font_lato = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(FONT_LATO, FONT_LATO_LENGTH, 16.0f, &defaultConfig);

	ImFontConfig titleConfig;
	titleConfig.FontDataOwnedByAtlas = false;
	title_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(FONT_LATO, FONT_LATO_LENGTH, 20.0f, &titleConfig);

	style->WindowMinSize = ImVec2(100, 100);
	style->GrabRounding = 0.f;
	style->WindowRounding = 3.0f;
	style->ScrollbarRounding = 3.f;
	style->FrameRounding = 3.f;
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->WindowPadding = ImVec2(WINDOW_PADDING, 4);
	style->FramePadding = ImVec2(12, 4);
	style->ItemInnerSpacing = ImVec2(5, 5);
	style->ScrollbarSize = 14;
	style->WindowMenuButtonPosition = ImGuiDir_Right;

	style->Colors[ImGuiCol_Text] = textColor;
	style->Colors[ImGuiCol_TextDisabled] = textColorDisabled;
	style->Colors[ImGuiCol_WindowBg] = ImVec4(darkColor.x, darkColor.y, darkColor.z, 0.95f);
	style->Colors[ImGuiCol_ChildBg] = darkColor;
	style->Colors[ImGuiCol_PopupBg] = darkColor;
	style->Colors[ImGuiCol_Border] = darkColor;
	style->Colors[ImGuiCol_BorderShadow] = darkColor;
	style->Colors[ImGuiCol_FrameBg] = mediumColor;
	style->Colors[ImGuiCol_FrameBgHovered] = mediumColorHover;
	style->Colors[ImGuiCol_FrameBgActive] = lightColor;
	style->Colors[ImGuiCol_TitleBg] = darkColor;
	style->Colors[ImGuiCol_TitleBgCollapsed] = darkColor;
	style->Colors[ImGuiCol_TitleBgActive] = mediumColor;
	style->Colors[ImGuiCol_MenuBarBg] = darkColor;
	style->Colors[ImGuiCol_ScrollbarBg] = mediumColor;
	style->Colors[ImGuiCol_ScrollbarGrab] = lightColor;
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = lightColorHover;
	style->Colors[ImGuiCol_ScrollbarGrabActive] = primaryColor;
	style->Colors[ImGuiCol_CheckMark] = primaryColor;
	style->Colors[ImGuiCol_SliderGrab] = lightColor;
	style->Colors[ImGuiCol_SliderGrabActive] = primaryColor;
	style->Colors[ImGuiCol_Button] = mediumColor;
	style->Colors[ImGuiCol_ButtonHovered] = mediumColorHover;
	style->Colors[ImGuiCol_ButtonActive] = primaryColor;
	style->Colors[ImGuiCol_Header] = mediumColor;
	style->Colors[ImGuiCol_HeaderHovered] = lightColorHover;
	style->Colors[ImGuiCol_HeaderActive] = lightColor;
	style->Colors[ImGuiCol_ResizeGrip] = mediumColor;
	style->Colors[ImGuiCol_ResizeGripHovered] = lightColor;
	style->Colors[ImGuiCol_ResizeGripActive] = primaryColor;
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
	style->Colors[ImGuiCol_Tab] = darkColor;
	style->Colors[ImGuiCol_TabActive] = primaryColor;
	style->Colors[ImGuiCol_TabHovered] = primaryColorHover;
	style->Colors[ImGuiCol_TabUnfocused] = mediumColor;
	style->Colors[ImGuiCol_TabUnfocusedActive] = mediumColorHover;
	style->Colors[ImGuiCol_Separator] = mediumColor;
	style->Colors[ImGuiCol_SeparatorHovered] = mediumColorHover;
	style->Colors[ImGuiCol_SeparatorActive] = primaryColor;
}


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


static void window_init_imgui(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	//io.ConfigWindowsResizeFromEdges = true;
	//io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsDark();
}


int window_init() {
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	return true;
}



window_t* window_create() {
	GLFWwindow* w = glfwCreateWindow(1280, 720, "Project EDA", nullptr, nullptr);
	if (w == nullptr)
		return nullptr;

	glfwMakeContextCurrent(w);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL((GLADloadfunc)glfwGetProcAddress) == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
	bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return nullptr;
	}

	int display_w, display_h;
	glfwGetFramebufferSize(w, &display_w, &display_h);

	glViewport(0, 0, display_w, display_h);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(w);
	glfwPollEvents();

	window_init_imgui(w);
	window_apply_imgui_style(w);
	window_t* window = new window_t;
	window->window = w;

	return window;
}


void window_render(GLFWwindow* window)
{
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}


int window_render(window_t* window, void (*fn_draw) (window_t*)) {

	if (glfwWindowShouldClose(window->window))
		return false;

	glfwPollEvents();

	glClearColor(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	int display_w, display_h;
	glfwGetFramebufferSize(window->window, &display_w, &display_h);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	gui_init_draw(window->window);

	if (fn_draw != nullptr)
		fn_draw(window);

	/*

	int width = 200;
	int height = 400;
	//ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::Begin("root", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(ImVec2(0, 0));

	//AlignForHeight(display_h);

	ImGui::PushFont(title_font);
	ImGuiDrawCenteredText("Menu");
	ImGui::PopFont();

	ImGui::Spacing();
	ImGui::Button("Jobs", ImVec2(width - WINDOW_PADDING_TOTAL, 30));
	ImGui::Button("BB", ImVec2(width - WINDOW_PADDING_TOTAL, 30));
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Button("Quit", ImVec2(width - WINDOW_PADDING_TOTAL, 30));
	ImGui::Spacing();

	//height = ImGui::GetContentRegionAvail().y;
	height = ImGui::GetWindowHeight();
	ImGui::SetWindowPos(ImVec2(display_w / 2 - width / 2, display_h / 2 - height / 2));

	ImGui::End();

	/*
	ImGui::Begin("hee");
	if (ImGui::Button("modal")) {
		ImGui::OpenPopup("EITA");
	}
	ImGui::Text("HEELLO");


	if (ImGui::BeginPopupModal("EITA")) {
		ImGui::Text("Lorem ipsum");
		ImGui::EndPopup();
	}

	ImGui::End();

	ImGui::EndFrame();

	*/
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window->window);

	return true;
}


void window_close(window_t* window) {
	glfwSetWindowShouldClose(window->window, true);
}


void window_destroy(window_t* window) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window->window);
	glfwTerminate();

	delete window;
}