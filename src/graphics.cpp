#include "smrfhck.hpp"

// to manage config file
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "INIReader.h"

#define SQRT2  1.41421356f
#define COS_45 (SQRT2 / 2.f)
#define SIN_45 COS_45


//TODO: add draw_circle
void draw_rect(float x, float y, float w, float h, ImColor *color)
{
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

    // printf("win: (%f, %f) (%f, %f)\n", vMin.x, vMin.y, vMax.x, vMax.y);

    // assume 0 >= x,y,w,h >= 1, 1 == full width/height
    float max_size = MIN((vMax.x - vMin.x) / SQRT2, (vMax.y - vMin.y) / SQRT2);
    x *= max_size;
    y *= max_size;
    w *= max_size;
    h *= max_size;

    // printf("max_size: %f\n", max_size);


    ImVec2 rect_a(x, y);
    ImVec2 rect_b(x + w, y);
    ImVec2 rect_c(x + w, y + h);
    ImVec2 rect_d(x, y + h);

    rect_a = ImRotate(rect_a, COS_45, SIN_45);
    rect_b = ImRotate(rect_b, COS_45, SIN_45);
    rect_c = ImRotate(rect_c, COS_45, SIN_45);
    rect_d = ImRotate(rect_d, COS_45, SIN_45);

    //window offset
    rect_a.x += vMin.x, rect_a.y += vMin.y;
    rect_b.x += vMin.x, rect_b.y += vMin.y;
    rect_c.x += vMin.x, rect_c.y += vMin.y;
    rect_d.x += vMin.x, rect_d.y += vMin.y;

    //compensate rotate
    ImVec2 win_center = ImVec2((vMax.x - vMin.x) / 2, (vMax.y - vMin.y) / 2);
    rect_a.x += win_center.x;//, rect_a.y += vMin.y;
    rect_b.x += win_center.x;//, rect_b.y += vMin.y;
    rect_c.x += win_center.x;//, rect_c.y += vMin.y;
    rect_d.x += win_center.x;//, rect_d.y += vMin.y;

    // printf("rect: (%f, %f) (%f, %f)\n",
    //        rect_a.x, rect_b.y,
    //     );


    ImGui::GetWindowDrawList()->AddQuadFilled(
        rect_a,
        rect_b,
        rect_c,
        rect_d,
        // ImGui::ColorConvertFloat4ToU32(ImVec4(1, .15, .15, 1)) );
        *color);
    // ImGui::GetWindowDrawList()->PushClipRectFullScreen();
}

static bool init_graphics(SDL_Window **window, SDL_GLContext *gl_context)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return FALSE;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    *window = SDL_CreateWindow(WINDOW_NAME,
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    *gl_context = SDL_GL_CreateContext(*window);
    SDL_GL_MakeCurrent(*window, *gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.IniFilename = NULL;
    std::ifstream ifs(CONFIG_FILE);
    std::string config_content(std::istreambuf_iterator<char>{ifs}, {});
    ImGui::LoadIniSettingsFromMemory(config_content.c_str(), config_content.size());
    ifs.close();
    INIReader reader(CONFIG_FILE);
    if (!reader.ParseError()) {
        for (auto it = g_colors.begin(); it != g_colors.end(); ++it) {
            long col = reader.GetInteger(CONFIG_COLOR_SECTION, it->first, 0);
            if (col > 0) {
                LOG_INFO("%s=%ld", it->first, col); // DEBUG
                g_colors[it->first] = ImColor((ImU32)col);
            }

            // std::string col = reader.Get(std::string(CONFIG_COLOR_SECTION), s, "nop");
            // // if (col != "nop") {
            //     LOG_INFO("%s=%s", it->first, col.c_str()); // DEBUG
            //     // g_colors[it->first] = ImColor((ImU32)col);
            // // }

        }
    }

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(*window, *gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return TRUE;
}

static void clean_graphics(SDL_Window *window, SDL_GLContext gl_context)
{
    const char *config_content = ImGui::SaveIniSettingsToMemory(NULL);
    std::ofstream ofs(CONFIG_FILE);
    ofs << config_content
        << "[" << CONFIG_COLOR_SECTION << "]" << std::endl;
    for (auto it = g_colors.begin(); it != g_colors.end(); ++it) {
        ofs << it->first << "=" << it->second << std::endl;
    }
    ofs.close();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

inline static bool handle_events(SDL_Window *window)
{
    static SDL_Event event;
    bool keep_going = TRUE;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            keep_going = FALSE;
        } else if (event.type == SDL_WINDOWEVENT
                   && event.window.event == SDL_WINDOWEVENT_CLOSE
                   && event.window.windowID == SDL_GetWindowID(window)) {
            keep_going = FALSE;
        }
    }

    if (keep_going) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    return keep_going;
}

inline static void render(SDL_Window *window)
{
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

bool render_loop(t_frame_callback frame_callback, void *data)
{
    SDL_Window *window;
    SDL_GLContext gl_context;

    if (!init_graphics(&window, &gl_context)) {
        return FALSE;
    }

    while (handle_events(window)) {
        frame_callback(data);
        render(window);
    }

    clean_graphics(window, gl_context);
    return TRUE;
}
