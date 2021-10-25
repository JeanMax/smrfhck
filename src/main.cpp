// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "smrfhck.hpp"

SDL_Window *window;
SDL_GLContext gl_context;
std::mutex mtx;

static BOOL is_shrine(DWORD id)
{
    return id == 2   // Shrine
        || id == 77  // Healthorama
        || id == 83  // Shrine
        || id == 84  // HealingWell
        || id == 85  // HealthShrine
        || id == 86  // Shrine
        || id == 93  // ManaShrine
        || id == 96  // HealthShrine
        || id == 97  // MagicShrine
        || id == 109 // Shrine HealthR
        || id == 111 // Fountain 1
        || id == 113 // Fountain 2 Well
        || id == 115 // Fountain 3
        || id == 116 // Snakewoman Shrine
        || id == 118 // Fountain 4
        || id == 120 // Healthshrine Dungeun
        || id == 123 // Innershrinehell2
        || id == 124 // Innershrinehell3
        || id == 130 // Pool Wilderness
        || id == 132 // Cathedralwell Inside
        || id == 133 // Shrine
        || id == 134 // Shrine
        || id == 135 // Shrine
        || id == 136 // Shrine
        || id == 137 // Desertwell Well Desert Tom
        || id == 138 // Cavewell Caves
        || id == 150 // Shrine1
        || id == 151 // Shrine4
        || id == 164 // Mana Well1
        || id == 165 // Mana Well2
        || id == 166 // Mana Well3 Tomb
        || id == 167 // Mana Well4 Harom
        || id == 168 // Mana Well5
        || id == 170 // Jungle Healwell
        || id == 172 // Health Well/shrine Desert
        || id == 173 // Mana Well/shrine Desert
        || id == 184 // Jungle Shrine2
        || id == 190 // Jungle Shrine3
        || id == 191 // Jungle Shrine4
        || id == 197 // Jungleshrine
        || id == 199 // Mafistoshrine
        || id == 200 // Mafistoshrine
        || id == 201 // Mafistoshrine
        || id == 202 // Mafistomana
        || id == 206 // Mafistohealth
        || id == 226 // Outershrinehell
        || id == 231 // Outershrinehell2
        || id == 232 // Outershrinehell3
        || id == 236 // Health Wellforhell
        || id == 249 // HellManaWell
        || id == 260 // Holyshrine
        || id == 275 // Health Shrine
        || id == 276 // Mana Shrine
        || id == 277 // Magic Shrine
        || id == 278 // Mana Shrine
        || id == 279 // Magic Shrine
        || id == 280 // Healthwell
        || id == 281 // Manawell
        || id == 282 // Shrine
        || id == 299 // MagicShrine
        || id == 300 // MagicShrine
        || id == 301 // HealthWell
        || id == 302 // ManaWell
        || id == 303 // MagicShrine
        || id == 319 // HealthWell
        || id == 320 // HealthWell
        || id == 322 // TombWell
        || id == 325 // MagicShrine
        || id == 343 // ManaWell
        || id == 344 // HealthWell
        || id == 361 // MagicShrine
        || id == 414 // Shrine
        || id == 415 // Shrine
        || id == 421 // Shrine 3
        || id == 422 // Mana Shrine
        || id == 423 // Health Shrine
        || id == 426 // Well
        || id == 427 // Shrine2
        || id == 428 // Shrine2
        || id == 464 // Health Shrine
        || id == 465 // Mana Shrine
        || id == 472 // IcecaveShrine2
        || id == 479 // IcecaveShrine2
        || id == 483 // ManaShrine
        || id == 484 // BaalHealthShrine
        || id == 488 // BaalMagicShrine
        || id == 491 // ManaShrine
        || id == 492 // HealthShrine
        || id == 493 // Well
        || id == 495 // Shrine3
        || id == 497 // Shrine3
        || id == 498 // BaalWell
        || id == 499 // BaalMagicShrine2
        || id == 503 // BaalMagicShrine3
        || id == 509 // TempleMagicShrine
        || id == 512 // TempleMagicShrine2
        || id == 513 // TempleWell
        || id == 519 // IcecaveWell
        || id == 520 // TempleMagicShrine3
        || id == 521 // TempleHealthShrine
        || id == 522 // TempleManaShrine
        || id == 539; // TempleWaypoint
        // || id == 12  // Rogue Fountain
        // || id == 31  // Fountain
}

static BOOL is_quest(DWORD id)
{
    return id == 8   // Tower Tome
        || id == 17  // StoneAlpha
        || id == 18  // StoneBeta
        || id == 19  // StoneGamma
        || id == 20  // StoneDelta
        || id == 21  // StoneLambda
        || id == 22  // StoneTheta
        || id == 30  // Inifuss Tree
        || id == 81  // Forest Altar
        || id == 108 // Malus
        || id == 152 // Holder For Horadric Staff
        || id == 193 // Lam Esen's Tome
        || id == 251 // Gidbinn Altar
        || id == 252 // Gidbinn Decoy
        || id == 354 // Horadric Cube Chest
        || id == 355 // Horadric Scroll Chest
        || id == 356 // Staff Of Kings Chest
        || id == 357 // Horazon's Journal
        || id == 376 // Forge Hell
        || id == 392 // Diablo Seal1
        || id == 393 // Diablo Seal2
        || id == 394 // Diablo Seal3
        || id == 395 // Diablo Seal4
        || id == 396 // Diablo Seal5
        || id == 404 // Compelling Orb
        || id == 405 // Khalim Chest
        || id == 406 // Khalim Chest
        || id == 407 // Khalim Chest
        || id == 458 // Temple Altar
        || id == 460 // Drehya Outside Town
        || id == 462 // Nihlathak Outside Town
        || id == 523 //  Blacksmith
        || id == 473 // Caged Fellow
        || id == 546 // AncientsAltar
        || id == 558; // Frozen Anya
}

static BOOL is_waypoint(DWORD id)
{
    return id == 119 // Waypoint Portal
        || id == 145 // Waypointi Inner Hell
        || id == 156 // Waypoint
        || id == 157 // WildernessWaypoint
        || id == 237 // Act3Waypoint Town
        || id == 238 // Waypointh
        || id == 288 // Waypoint Celler
        || id == 323 // SewerWaypoint
        || id == 324 // TravincalWaypoint
        || id == 398 // PandamoniaWaypoint
        || id == 429 // Waypoint
        || id == 398 // PandamoniaWaypoint
        || id == 402 // VallyWaypoint
        || id == 494 // BaalWaypoint
        || id == 496 // WildernessWaypoint
        || id == 511; // IcecaveWaypoint
}

static BOOL is_transit(DWORD id)
{
    return id == 74  // Trap Door
        || id == 194 // Stairsl
        || id == 195 // Stairsr
        || id == 244 // Sewers
        || id == 246 // Sewers
        || id == 262 // Cathedral
        || id == 263 // Jail
        || id == 264 // Jail
        || id == 265 // Jail
        || id == 298 // Arcane Portal
        || id == 342 // Hellgate
        || id == 366 // Sewer Quest Stairs
        || id == 367 // Sewer Quest Lever
        || id == 386 // Stairsr
        || id == 547 // Worldstone Level1
        || id == 557 // Baal's Lair
        || id == 563 // Worldstone Chamber
        || id == 564 // GlacialCaves Level1
        || id == 565 // Last Cinematic
        || id == 566 // Harrogath LastPortal
        || id == 569; // ThroneOfDest Portal
        // || id == 59  // Town Portal
        // || id == 60  // Permanent Town Portal
        // || id == 189 // Cain Portal
        // || id == 268 // Wirt's Body
        // || id == 377 // Portal To Guild
        // || id == 451 // Hellgate
}

static BOOL is_interesting_preset(DWORD id)
{
    return is_quest(id)
        || is_shrine(id)
        || is_waypoint(id)
        || is_transit(id)
        || id == 267; // Bank
}

static void draw_rect(float x, float y, float w, float h, ImColor *color)
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

static BOOL init()
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
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("smrfhck", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return TRUE;
}

inline static void render(void)
{
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

static void finit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

static void game_refresher()
{
    std::unique_lock<std::mutex> lck (mtx, std::defer_lock);

    while (42) {
        {
            // std::unique_lock<std::mutex> lck (mtx, std::defer_lock);
            lck.lock();
            refresh();
            lck.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


int main(int, char**)
{
    GameState *game = NULL;
    while (!(game = refresh())) {
        sleep(1);
    }

    std::thread game_thread(game_refresher);
    std::unique_lock<std::mutex> lck (mtx, std::defer_lock);

    if (!init()) {
        EXIT_FAILURE;
    }

// Our state
    ImColor red(COLOR_RED_1, 0.8f);
    ImColor green(COLOR_GREEN_1, 0.8f);
    ImColor blue(COLOR_BLUE_1, 0.8f);

    ImColor yellow(COLOR_YELLOW_1, 0.8f);
    ImColor cyan(COLOR_CYAN_1, 0.8f);
    ImColor magenta(COLOR_MAGENTA_1, 0.8f);

    ImColor white(COLOR_WHITE_1, 0.8f);
    ImColor black(COLOR_BLACK_1, 0.8f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();


        // bool show_demo_window = true;
        // ImGui::ShowDemoWindow(&show_demo_window);

        // main
        ImGui::Begin("smrfhck");

        // if (lck.try_lock()) {
        {
            lck.lock();
            DWORD max_size = MAX(game->level->dwSizeX, game->level->dwSizeY) * 5.f;
            draw_rect(0, 0,
                      (float)game->level->dwSizeX * 5.f / (float)max_size,
                      (float)game->level->dwSizeY * 5.f / (float)max_size,
                      &red);

            Room2 *r2 = game->level->pRoom2First;
            while (r2) {
                draw_rect((float)(r2->dwPosX - game->level->dwPosX) * 5.f / (float)max_size,
                          (float)(r2->dwPosY - game->level->dwPosY) * 5.f / (float)max_size,
                          (float)r2->dwSizeX * 5.f / (float)max_size,
                          (float)r2->dwSizeY * 5.f / (float)max_size,
                          &blue);

                ImColor *col[2] = {&cyan, &yellow};
                for (PresetUnit *pu = r2->pPreset; pu; pu = pu->pPresetNext) {
                    // LOG_INFO("preset: %d (%d, %d)", pu->dwTxtFileNo, pu->dwPosX, pu->dwPosY);
                    if (pu->dwType < 3 && !is_interesting_preset(pu->dwTxtFileNo)) {
                        // LOG_INFO("preset %d", pu->dwTxtFileNo); // DEBUG
                        // draw_rect(((float)(r2->dwPosX - game->level->dwPosX) * 5.f + (float)pu->dwPosX) / (float)max_size,
                        //           ((float)(r2->dwPosY - game->level->dwPosY) * 5.f  + (float)pu->dwPosY) / (float)max_size,
                        //           0.0125,
                        //           0.0125,
                        //           &red);
                        continue;
                    }
                    // LOG_INFO("------------------------------"); // DEBUG

                    draw_rect(((float)(r2->dwPosX - game->level->dwPosX) * 5.f + (float)pu->dwPosX) / (float)max_size,
                              ((float)(r2->dwPosY - game->level->dwPosY) * 5.f  + (float)pu->dwPosY) / (float)max_size,
                              0.025,
                              0.025,
                              pu->dwType < 3 ? col[pu->dwType - 1] : &green);
                }

                // Room1 *r1 = r2->pRoom1;
                // while (r1) {
                //     draw_rect((float)(r1->dwPosX - game->level->dwPosX) * 5.f / (float)max_size,
                //               (float)(r1->dwPosY - game->level->dwPosY) * 5.f / (float)max_size,
                //               (float)r1->dwSizeX * 5.f / (float)max_size,
                //               (float)r1->dwSizeY * 5.f / (float)max_size,
                //               &yellow);

                //     r1 = r1->pRoomNext;
                // }


                r2 = r2->pRoom2Next;
            }


            draw_rect(((float)game->path.xPos - (float)game->level->dwPosX * 5.f) / (float)max_size,
                      ((float)game->path.yPos - (float)game->level->dwPosY * 5.f) / (float)max_size,
                      0.05, 0.05,
                      &green);


            lck.unlock();
        }

        // LOG_INFO("max %d", max_size);


        // ImColor *colors[3] = {&red, &green, &blue};
        // unsigned int col_idx = 0;
        // for (float x = 0; x < 1; x += 0.1) {
        //     for (float y = 0; y < 1; y += 0.1) {
        //         draw_rect(x, y, 0.1, 0.1, colors[col_idx++ % 3]);
        //     }
        // }

        // draw_rect(0.1, 0.1, 1, 1, &red);
        // draw_rect(0, 0, 0.5, 0.5, &green);

        ImGui::End();

        render();
    }

// TODO: kill thread
    finit();

    return EXIT_SUCCESS;
}
