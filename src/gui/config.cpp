#include "gui/gui.hpp"

// to manage config file
#include <string>
#include <iostream>
#include <fstream>
#include "INIReader.h"


void read_window_title_config(GameState *game)
{
    INIReader reader(CONFIG_FILE);
    if (!reader.ParseError()) {
        std::string toto = reader.Get(CONFIG_WINDOW_SECTION, "window_title", "");
		memcpy(game->window_title_setting, toto.c_str(), toto.size() + 1);
    }
}

int *read_window_config()
{
    static int win[4] = {
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT
    };
    INIReader reader(CONFIG_FILE);
    if (!reader.ParseError()) {
        int i = (int)reader.GetInteger(CONFIG_WINDOW_SECTION, "x", -1);
        if (i >= 0) {
            win[0] = i;
        }
        i = (int)reader.GetInteger(CONFIG_WINDOW_SECTION, "y", -1);
        if (i >= 0) {
            win[1] = i;
        }
        i = (int)reader.GetInteger(CONFIG_WINDOW_SECTION, "w", -1);
        if (i >= 0) {
            win[2] = i;
        }
        i = (int)reader.GetInteger(CONFIG_WINDOW_SECTION, "h", -1);
        if (i >= 0) {
            win[3] = i;
        }
    }
    return (int *)&win;
}

void read_config()
{
    std::ifstream ifs(CONFIG_FILE);
    std::string config_content(std::istreambuf_iterator<char>{ifs}, {});
    ImGui::LoadIniSettingsFromMemory(config_content.c_str(), config_content.size());
    ifs.close();
    INIReader reader(CONFIG_FILE);
    if (!reader.ParseError()) {
        for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
            std::string color = reader.Get(CONFIG_COLOR_SECTION, it->first, "");
            if (color.size()) {
                //Get+stoll instead of GetInt to avoid overflow
                g_settings[it->first].color = ImColor((ImU32)std::stoll(color));
            }
            float size = reader.GetFloat(CONFIG_SIZE_SECTION, it->first, -1.f);
            if (size >= 0) {
                g_settings[it->first].size = size;
            }
            long is_circle = reader.GetInteger(CONFIG_SHAPE_SECTION, it->first, -1);
            if (is_circle >= 0) {
                g_settings[it->first].is_circle = is_circle;
            }
        }
    }
}

void write_config(SDL_Window *window, GameState *game)
{
    const char *config_content = ImGui::SaveIniSettingsToMemory(NULL);
    std::ofstream ofs(CONFIG_FILE);

    ofs << config_content
        << "[" << CONFIG_COLOR_SECTION << "]" << std::endl;
    for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
        ofs << it->first << "=" << it->second.color << std::endl;
    }
    ofs << std::endl
        << "[" << CONFIG_SIZE_SECTION << "]" << std::endl;
    for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
        ofs << it->first << "=" << it->second.size << std::endl;
    }
    ofs << std::endl
        << "[" << CONFIG_SHAPE_SECTION << "]" << std::endl;
    for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
        ofs << it->first << "=" << it->second.is_circle << std::endl;
    }

    int x, y, w, h;
    SDL_GetWindowPosition(window, &x, &y);
    SDL_GetWindowSize(window, &w, &h);
    ofs << std::endl
        << "[" << CONFIG_WINDOW_SECTION << "]" << std::endl;
    ofs << "x=" << x << std::endl;
    ofs << "y=" << y << std::endl;
    ofs << "w=" << w << std::endl;
    ofs << "h=" << h << std::endl;
    ofs << "window_title=" << game->window_title_setting << std::endl;

    ofs.close();
}
