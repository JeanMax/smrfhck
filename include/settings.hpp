#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

#include <imgui.h>
#include <map>
#include <string>


#define PROG_NAME "smrfhck"


#define LOG_FILE PROG_NAME ".log"

// graphics.cpp
#define WINDOW_NAME   PROG_NAME
#define WINDOW_WIDTH  480
#define WINDOW_HEIGHT 600

// config.cpp
#define CONFIG_FILE PROG_NAME ".ini"
#define CONFIG_COLOR_SECTION "Colors"
#define CONFIG_SIZE_SECTION "Size"
#define CONFIG_SHAPE_SECTION "Shape"
#define CONFIG_WINDOW_SECTION "Main"


// settings.cpp
#define COLOR_BLACK_1   0.141f,  0.141f, 0.141f  //fg
#define COLOR_BLACK_2   0.376f,  0.376f, 0.376f
#define COLOR_RED_1     0.678f,  0.208f, 0.133f
#define COLOR_RED_2     0.804f,  0.333f, 0.259f
#define COLOR_GREEN_1   0.0667f, 0.514f, 0.282f
// #define COLOR_GREEN_2   0.208f,  0.596f, 0.349f
#define COLOR_GREEN_2   0.317f,  0.686f, 0.435f
#define COLOR_YELLOW_1  0.729f,  0.729f, 0.212f
#define COLOR_YELLOW_2  0.722f,  0.718f, 0.294f
#define COLOR_BLUE_1    0.259f,  0.467f, 0.694f
#define COLOR_BLUE_2    0.322f,  0.561f, 0.82f
#define COLOR_MAGENTA_1 0.678f,  0.353f, 0.824f
#define COLOR_MAGENTA_2 0.749f,  0.478f, 0.867f
#define COLOR_CYAN_1    0.0f,    0.529f, 0.51f
#define COLOR_CYAN_2    0.157f,  0.69f,  0.612f
#define COLOR_WHITE_1   0.741f,  0.741f, 0.702f
#define COLOR_WHITE_2   0.8f,    0.8f,   0.8f    //bg


#define CURRENT_LEVEL_SETTING_STR         "Current Level"
#define EXPLORED_AREAS_SETTING_STR        "Explored Areas"
#define NEXT_LEVEL_SETTING_STR            "Next Level"
#define PREV_LEVEL_SETTING_STR            "Previous Level"
#define PLAYER_SETTING_STR                "Player (it's you!)"
#define OTHER_PLAYERS_SETTING_STR         "Other Players"
#define MONSTER_SUPER_SETTING_STR         "Monster Super"
#define MONSTER_MINION_SETTING_STR        "Monster Minion"
#define MONSTER_BOSS_SETTING_STR          "Monster Boss"
#define MONSTER_CHAMP_SETTING_STR         "Monster Champ"
#define MONSTER_NORMAL_SETTING_STR        "Monster Normal"
#define NPC_PRESET_SETTING_STR            "Npc/Monster Preset"
#define LEVEL_CONNECTION_UP_SETTING_STR   "Level Connection Up"
#define LEVEL_CONNECTION_DOWN_SETTING_STR "Level Connection Down"
#define WEIRD_CONNECTION_SETTING_STR      "Weird Connection"
#define WAYPOINT_SETTING_STR              "Waypoint"
#define SHRINE_SETTING_STR                "Shrine"
#define QUEST_SETTING_STR                 "Quest"
#define UNKNOWN_SETTING_STR               "Unknown"
#define BORING_SETTING_STR                "Not Interesting"


typedef struct setting Setting;
struct setting {
    ImColor color;
    float size;
    bool is_circle;
};

extern std::map<std::string, Setting> g_settings;


#endif
