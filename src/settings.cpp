#include "settings.hpp"


Setting g_settings[MAX_SETTINGS] = {
    {.name=ENTRANCE_UP_SETTING_STR,
         .color=ImColor(COLOR_GREEN_2, 0.8f), .size=0.025f, .is_circle=0},
    {.name=ENTRANCE_DOWN_SETTING_STR,
         .color=ImColor(COLOR_GREEN_1, 0.8f), .size=0.025f, .is_circle=1},

    {.name=BOSS_ZONE_SETTING_STR,
         .color=ImColor(COLOR_RED_1, 0.4f), .size=0.035f, .is_circle=1},
    {.name=WAYPOINT_SETTING_STR,
         .color=ImColor(COLOR_MAGENTA_1, 0.8f), .size=0.030f, .is_circle=0},
    {.name=SHRINE_SETTING_STR,
         .color=ImColor(COLOR_YELLOW_1, 0.8f), .size=0.025f, .is_circle=0},
    {.name=QUEST_SETTING_STR,
         .color=ImColor(COLOR_BLUE_1, 0.8f), .size=0.025f, .is_circle=0},

    {.name=PLAYER_SETTING_STR,
         .color=ImColor(COLOR_CYAN_1, 0.8f), .size=0.015f, .is_circle=1},
    {.name=OTHER_PLAYERS_SETTING_STR,
         .color=ImColor(COLOR_CYAN_2, 0.8f), .size=0.010f, .is_circle=1},

    {.name=MONSTER_SUPER_SETTING_STR,
         .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1},
    {.name=MONSTER_BOSS_SETTING_STR,
         .color=ImColor(COLOR_ORANGE_1, 0.8f), .size=0.007f, .is_circle=1},
    {.name=MONSTER_MINION_SETTING_STR,
         .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1},
    {.name=MONSTER_CHAMP_SETTING_STR,
         .color=ImColor(COLOR_MAGENTA_2, 0.8f), .size=0.007f, .is_circle=1},
    {.name=MONSTER_NORMAL_SETTING_STR,
         .color=ImColor(COLOR_WHITE_2, 0.8f), .size=0.007f, .is_circle=1},

    {.name=MISSILE_SETTING_STR,
         .color=ImColor(COLOR_WHITE_1, 0.1f), .size=0.007f, .is_circle=1},

    {.name=BORING_SETTING_STR,
         .color=ImColor(COLOR_WHITE_1, 0.1f), .size=0.007f, .is_circle=1},

    {.name=EXPLORED_AREAS_SETTING_STR,
         .color=ImColor(COLOR_BLACK_2, 0.8f), .size=0.f, .is_circle=0},
    {.name=CURRENT_LEVEL_SETTING_STR,
         .color=ImColor(COLOR_BLACK_1, 0.8f), .size=0.f, .is_circle=0},
    {.name=NEXT_LEVEL_SETTING_STR,
         .color=ImColor(COLOR_RED_1, 0.8f), .size=0.f, .is_circle=0},
    {.name=PREV_LEVEL_SETTING_STR,
         .color=ImColor(COLOR_RED_2, 0.8f), .size=0.f, .is_circle=0},
};
