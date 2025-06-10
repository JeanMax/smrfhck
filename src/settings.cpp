#include "settings.hpp"


std::map<std::string, Setting> g_settings {
    {CURRENT_LEVEL_SETTING_STR, {
            .color=ImColor(COLOR_BLACK_1, 0.8f), .size=0.f, .is_circle=0}},
    {EXPLORED_AREAS_SETTING_STR, {
            .color=ImColor(COLOR_BLACK_2, 0.8f), .size=0.f, .is_circle=0}},
    {NEXT_LEVEL_SETTING_STR, {
            .color=ImColor(COLOR_RED_1, 0.8f), .size=0.f, .is_circle=0}},
    {PREV_LEVEL_SETTING_STR, {
            .color=ImColor(COLOR_RED_2, 0.8f), .size=0.f, .is_circle=0}},
    {PLAYER_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_1, 0.8f), .size=0.007f, .is_circle=1}},
    {OTHER_PLAYERS_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1}},
    {MONSTER_SUPER_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1}},
    {MONSTER_BOSS_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1}},
    {MONSTER_MINION_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1}},
    {MONSTER_CHAMP_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1}},
    {MONSTER_NORMAL_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_2, 0.8f), .size=0.007f, .is_circle=1}},
    {NPC_PRESET_SETTING_STR, {
            .color=ImColor(COLOR_RED_1, 0.8f), .size=0.005f, .is_circle=1}},  //TODO: rename to "super zone"
    {LEVEL_CONNECTION_UP_SETTING_STR, {
            .color=ImColor(COLOR_GREEN_2, 0.8f), .size=0.025f, .is_circle=0}},
    {LEVEL_CONNECTION_DOWN_SETTING_STR, {
            .color=ImColor(COLOR_GREEN_1, 0.8f), .size=0.025f, .is_circle=0}},
    {WEIRD_CONNECTION_SETTING_STR, {
            .color=ImColor(COLOR_CYAN_1, 0.8f), .size=0.025f, .is_circle=0}},  //TODO: rename to "boss zone"
    {WAYPOINT_SETTING_STR, {
            .color=ImColor(COLOR_MAGENTA_1, 0.8f), .size=0.025f, .is_circle=0}},
    {SHRINE_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_1, 0.8f), .size=0.025f, .is_circle=0}},
    {QUEST_SETTING_STR, {
            .color=ImColor(COLOR_BLUE_1, 0.8f), .size=0.025f, .is_circle=0}},
    {UNKNOWN_SETTING_STR, {
            .color=ImColor(COLOR_WHITE_1, 0.8f), .size=0.025f, .is_circle=0}},
    {BORING_SETTING_STR, {
            .color=ImColor(COLOR_WHITE_1, 0.1f), .size=0.025f, .is_circle=0}},
};  //TODO: add cool default colors / shape / size
