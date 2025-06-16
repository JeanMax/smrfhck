#include "smrfhck.hpp"

#include <thread>
#include <chrono>

#include <fcntl.h>


std::atomic_bool g_exit_now = false;


static bool redirect_output_to_file(const char *filename)
{
    int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 00644);
    if (fd < 0) {
        LOG_ERROR("Can't redirect output to logfile (will you ever read this?!)");
        return FALSE;
    }
    return dup2(fd, STDOUT_FILENO) >= 0
        && dup2(fd, STDERR_FILENO) >= 0;
}

static const char *get_date_str()
{
    time_t tt;
    time(&tt);
    return asctime(localtime(&tt));
}

inline static void frame_callback(void *data)
{
    GameState *game = (GameState *)data;

    ImGui::SetNextWindowPos(ImVec2(10, 25), //TODO: adjust default windows position
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH - 20, WINDOW_HEIGHT - 35),
                             ImGuiCond_FirstUseEver);
    if (ImGui::Begin(PROG_NAME)) {
        pthread_mutex_lock(&game->mutex);
        if (!game->level) {
            ImGui::Text("%s", *game->status ? game->status : "Loading...");
        } else {
            draw_map(game);
        }
        pthread_mutex_unlock(&game->mutex);
    }
    ImGui::End();

    static bool show_menu = true;
    if (show_menu) {
        ImGui::SetNextWindowPos(ImVec2(10, 3),
                                ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowCollapsed(TRUE, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("settings", &show_menu, 0)) {
            pthread_mutex_lock(&game->mutex);
            draw_menu(game);
            pthread_mutex_unlock(&game->mutex);
        }
        ImGui::End();
    }

    static bool show_debug = true;
    if (show_debug) {
        ImGui::SetNextWindowPos(ImVec2(350, 3),
                                ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(120, WINDOW_HEIGHT / 2.),
                                 ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowCollapsed(TRUE, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("debug", &show_debug, 0)) {
            pthread_mutex_lock(&game->mutex);
            if (!game->level) {
                ImGui::Text("Loading...");
            } else {
                draw_debug(game);
            }
            pthread_mutex_unlock(&game->mutex);
        }
        ImGui::End();
    }

    // static bool show_demo_window = true;   // DEBUG
    // if (show_demo_window) {
    //     ImGui::ShowDemoWindow(&show_demo_window);
    //     ImGui::ShowDebugLogWindow();
    // }
}

static void game_refresher(void *data)
{
    GameState *game = (GameState *)data;
    std::chrono::duration fail_delay = std::chrono::milliseconds(1000);
#ifdef NDEBUG
    std::chrono::duration frame_delay = std::chrono::milliseconds(500);
#else
    std::chrono::duration frame_delay = std::chrono::milliseconds(100);
    // ideally you'd want 9fps, so less than 111ms sleep
#endif

    while (!g_exit_now) {
        auto tick = std::chrono::high_resolution_clock::now();
        bool update_success = update_game_state(game);
        auto tack = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(tack - tick);
        LOG_DEBUG("frame took %dms", duration.count());
        if (!update_success) {
            std::this_thread::sleep_for(fail_delay);
        } else {
            std::this_thread::sleep_for(frame_delay - duration);
        }
    }
    g_exit_now = false;
}

int main(int, char**)
{
    int ret = EXIT_SUCCESS;

    redirect_output_to_file(LOG_FILE);
    //TODO: log current version
    LOG_INFO("\n\n%s *Starting smrfhck*", get_date_str());

    GameState game;
    init_game_state(&game);

    std::thread game_thread(game_refresher, (void *)&game);
    game_thread.detach();

    if (!render_loop(frame_callback, (void *)&game)) {
        ret = EXIT_FAILURE;
    }

    g_exit_now = true;
#ifdef NDEBUG
    while (g_exit_now) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    destroy_game_state(&game);
    // this is basically to make sure we didn't leak any memory
#endif

    return ret;
}
