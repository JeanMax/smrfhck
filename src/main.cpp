#include "smrfhck.hpp"

#define CURRENT_LEVEL_ROOM2_SETTING_STR   "Current Level's Room2"
#define OTHER_LEVEL_ROOM2_SETTING_STR     "Other Level's Room2"
#define OTHER_LEVEL_SETTING_STR           "Other Level"
#define PLAYER_SETTING_STR                "Player (it's you!)"
#define NPC_PRESET_SETTING_STR            "Npc/Monster Preset"
#define LEVEL_CONNECTION_UP_SETTING_STR   "Level Connection Up"
#define LEVEL_CONNECTION_DOWN_SETTING_STR "Level Connection Down"
#define WEIRD_CONNECTION_SETTING_STR      "Weird Connection"
#define WAYPOINT_SETTING_STR              "Waypoint"
#define SHRINE_SETTING_STR                "Shrine"
#define QUEST_SETTING_STR                 "Quest"
#define UNKNOWN_SETTING_STR               "Unknown"
#define BORING_SETTING_STR                "Not Interesting"

std::map<const char *, Setting> g_settings {
    {CURRENT_LEVEL_ROOM2_SETTING_STR, {
            .color=ImColor(COLOR_BLACK_1, 0.8f), .size=0.f, .is_circle=0}},
    {OTHER_LEVEL_ROOM2_SETTING_STR, {
            .color=ImColor(COLOR_RED_1, 0.8f), .size=0.f, .is_circle=0}},
    {OTHER_LEVEL_SETTING_STR, {
            .color=ImColor(COLOR_RED_1, 0.8f), .size=0.f, .is_circle=0}},
    {PLAYER_SETTING_STR, {
            .color=ImColor(COLOR_GREEN_1, 0.8f), .size=0.04f, .is_circle=0}},
    {NPC_PRESET_SETTING_STR, {
            .color=ImColor(COLOR_RED_1, 0.8f), .size=0.025f, .is_circle=0}},
    {LEVEL_CONNECTION_UP_SETTING_STR, {
            .color=ImColor(COLOR_GREEN_2, 0.8f), .size=0.025f, .is_circle=0}},
    {LEVEL_CONNECTION_DOWN_SETTING_STR, {
            .color=ImColor(COLOR_GREEN_1, 0.8f), .size=0.025f, .is_circle=0}},
    {WEIRD_CONNECTION_SETTING_STR, {
            .color=ImColor(COLOR_CYAN_1, 0.8f), .size=0.025f, .is_circle=0}},
    {WAYPOINT_SETTING_STR, {
            .color=ImColor(COLOR_MAGENTA_1, 0.8f), .size=0.025f, .is_circle=0}},
    {SHRINE_SETTING_STR, {
            .color=ImColor(COLOR_YELLOW_1, 0.8f), .size=0.025f, .is_circle=0}},
    {QUEST_SETTING_STR, {
            .color=ImColor(COLOR_BLUE_1, 0.8f), .size=0.025f, .is_circle=0}},
    {UNKNOWN_SETTING_STR, {
            .color=ImColor(COLOR_WHITE_1, 0.8f), .size=0.025f, .is_circle=0}},
    {BORING_SETTING_STR, {
            .color=ImColor(COLOR_WHITE_1, 0.0f), .size=0.025f, .is_circle=0}},
};


static dword get_level_size(GameState *game)
{
    dword max = 0;
    Room2 *r2 = game->level->pRoom2First;
    while (r2) {
        max = MAX(max, (r2->dwPosX - game->level->dwPosX + r2->dwSizeX) * 5);
        max = MAX(max, (r2->dwPosY - game->level->dwPosY + r2->dwSizeY) * 5);
        r2 = r2->pRoom2Next;
    }
    return max;
}

static void draw_level_connection(GameState *game, float max_size)
{
    Level *lvl;
    dword act = act_from_area(game->level->dwLevelNo);
    for (int i = 0; i < MAX_AREA; i++) {
        lvl = game->all_levels[i];
        if (!lvl
            || lvl->dwLevelNo == game->level->dwLevelNo
            || act_from_area(lvl->dwLevelNo) != act) {
            continue;
        }

        if (lvl->pRoom2First) {
            for (Room2 *r = lvl->pRoom2First; r; r = r->pRoom2Next) {
                draw_rect(((float)r->dwPosX - (float)game->level->dwPosX) / max_size,
                          ((float)r->dwPosY - (float)game->level->dwPosY) / max_size,
                          (float)r->dwSizeX / max_size,
                          (float)r->dwSizeY / max_size,
                          &g_settings[OTHER_LEVEL_ROOM2_SETTING_STR].color);
            }
        } else {
            // LOG_INFO("no room2 in lvl %d", lvl->dwLevelNo);
            draw_rect(((float)lvl->dwPosX - (float)game->level->dwPosX) / max_size,
                      ((float)lvl->dwPosY - (float)game->level->dwPosY) / max_size,
                      (float)lvl->dwSizeX / max_size,
                      (float)lvl->dwSizeY / max_size,
                      &g_settings[OTHER_LEVEL_SETTING_STR].color); //level
        }

    }
}

static void draw_presets(Room2 *r2, Level *level, float max_size)
{
    ImColor *color = NULL;
    float size = 0.f;

    for (PresetUnit *pu = r2->pPreset; pu; pu = pu->pPresetNext) {
        if (pu->dwType == 1) { //monster/npc
            if (is_uninteresting_unit(pu->dwTxtFileNo)) {
                continue;
            }
            color = &g_settings[NPC_PRESET_SETTING_STR].color;
            size = g_settings[NPC_PRESET_SETTING_STR].size;
            // if (pu->dwTxtFileNo < 734) { //super unique boss
            //     LOG_INFO("preset: id=%d type=%d (%d, %d)",
            //              pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);
            // }
        } else if (pu->dwType == 2) {  //object
            if (is_waypoint(pu->dwTxtFileNo)) {
                color = &g_settings[WAYPOINT_SETTING_STR].color;
                size = g_settings[WAYPOINT_SETTING_STR].size;
            } else if (is_quest(pu->dwTxtFileNo)) {
                color = &g_settings[QUEST_SETTING_STR].color;
                size = g_settings[QUEST_SETTING_STR].size;
            } else if (is_shrine(pu->dwTxtFileNo)) {
                color = &g_settings[SHRINE_SETTING_STR].color;
                size = g_settings[SHRINE_SETTING_STR].size;
            } else if (is_transit(pu->dwTxtFileNo)) {
                color = &g_settings[WEIRD_CONNECTION_SETTING_STR].color;
                size = g_settings[WEIRD_CONNECTION_SETTING_STR].size;
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                // continue;
                color = &g_settings[BORING_SETTING_STR].color;
                size = g_settings[BORING_SETTING_STR].size;
            }
        } else if (pu->dwType == 5) {  //tiles
            if (is_backward_tile(pu->dwTxtFileNo)) {
                color = &g_settings[LEVEL_CONNECTION_UP_SETTING_STR].color; //probably not what you're searching for
                size = g_settings[LEVEL_CONNECTION_UP_SETTING_STR].color; //probably not what you're searching size
            } else {
                color = &g_settings[LEVEL_CONNECTION_DOWN_SETTING_STR].color;
                size = g_settings[LEVEL_CONNECTION_DOWN_SETTING_STR].size;
            }
        } else { // ???
            color = &g_settings[UNKNOWN_SETTING_STR].color;
            size = g_settings[UNKNOWN_SETTING_STR].size;
            LOG_INFO("preset: id=%d type=%d (%d, %d)",
                     pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);

        }
        draw_rect(((float)r2->dwPosX - (float)level->dwPosX + (float)pu->dwPosX / 5.f) / max_size - size / 2,
                  ((float)r2->dwPosY - (float)level->dwPosY + (float)pu->dwPosY / 5.f) / max_size - size / 2,
                  size,
                  size,
                  color);

    }

}

inline static void draw_map(GameState *game)
{
    // float max_size = (float)MAX(game->level->dwSizeX, game->level->dwSizeY);
    float max_size = (float)get_level_size(game) / 5.f;
    // draw_rect(0, 0,
    //           (float)game->level->dwSizeX / max_size,
    //           (float)game->level->dwSizeY / max_size,
    //           &red); //level

    draw_level_connection(game, max_size);

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pRoom2Next) {
        draw_rect(((float)r2->dwPosX - (float)game->level->dwPosX) / max_size,
                  ((float)r2->dwPosY - (float)game->level->dwPosY) / max_size,
                  (float)r2->dwSizeX / max_size,
                  (float)r2->dwSizeY / max_size,
                  &g_settings[CURRENT_LEVEL_ROOM2_SETTING_STR].color); //room2



        // for (Room1 *r1 = r2->pRoom1; r1; r1 = r1->pRoomNext) {
        //     draw_rect((float)(r1->dwPosX - game->level->dwPosX) / max_size,
        //               (float)(r1->dwPosY - game->level->dwPosY) / max_size,
        //               (float)r1->dwSizeX / max_size,
        //               (float)r1->dwSizeY / max_size,
        //               &yellow);
        // }
    }

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pRoom2Next) {
        //2nd iterate so it's drawn over room2
        draw_presets(r2, game->level, max_size);
    }

    float player_size = g_settings[PLAYER_SETTING_STR].size;

    draw_rect(((float)game->player.pPath->xPos / 5.f - (float)game->level->dwPosX) / max_size - player_size / 2,
              ((float)game->player.pPath->yPos / 5.f - (float)game->level->dwPosY) / max_size - player_size / 2,
              player_size,
              player_size,
              &g_settings[PLAYER_SETTING_STR].color); //player

}

////////////////////////////////////////////////////////////////////////////////

inline static void draw_debug(GameState *game)
{
   for (PresetUnit *pu = game->level->pRoom2First->pPreset; pu; pu = pu->pPresetNext) {
        if (pu->dwType == 1) { //monster/npc
            if (is_uninteresting_unit(pu->dwTxtFileNo)) {
                continue;
            }
            // color = &red;
        } else if (pu->dwType == 2) {  //object
            if (is_waypoint(pu->dwTxtFileNo)) {
                // color = &magenta;
            } else if (is_quest(pu->dwTxtFileNo)) {
                // color = &blue;
            } else if (is_shrine(pu->dwTxtFileNo)) {
                // color = &yellow;
            } else if (is_transit(pu->dwTxtFileNo)) {
                // color = &cyan;
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                continue;
            }
        } else if (pu->dwType == 5) {  //tiles
            if (is_backward_tile(pu->dwTxtFileNo)) {
                // color = &green_bis; //probably not what you're searching for
            } else {
                // color = &green;
            }
        } else { // ???
            // color = &white;
        }
        // ;
    }
}

////////////////////////////////////////////////////////////////////////////////

inline static void draw_settings(void)
{
    // static bool check = true;
    int i = 0;

    // ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
        ImGui::ColorEdit4(it->first,
                          (float *)&it->second.color,
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        ImGui::Text("%s", it->first);
        if (strcmp(it->first, CURRENT_LEVEL_ROOM2_SETTING_STR)
                && strcmp(it->first, OTHER_LEVEL_ROOM2_SETTING_STR)
                && strcmp(it->first, OTHER_LEVEL_SETTING_STR)) {
            ImGui::SameLine(200);
            ImGui::PushItemWidth(90);
            ImGui::PushID(i++);
            ImGui::InputFloat("", &it->second.size, 0.001f, 0.1f, "%.3f");
            ImGui::PopID();

            // ImGui::Checkbox("checkbox", &check);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

inline static void frame_callback(void *data)
{
    GameState *game = (GameState *)data;
    static bool p_open = true;

    if (ImGui::Begin("smrfhck")) {
        pthread_mutex_lock(&game->mutex);
        if (!game->level) {
            ImGui::Text("Loading...");
        } else {
            draw_map(game);
        }
        pthread_mutex_unlock(&game->mutex);
    }
    ImGui::End();

    if (p_open) {
        // if (ImGui::Begin("debug", &p_open, 0)) {
        //     pthread_mutex_lock(&game->mutex);
        //     if (!game->level) {
        //         ImGui::Text("Loading...");
        //     } else {
        //         draw_debug(game);
        //     }
        //     pthread_mutex_unlock(&game->mutex);
        // }
        // ImGui::End();

        if (ImGui::Begin("settings", &p_open, 0)) {
            draw_settings();
        }
        ImGui::End();
    }

    static bool show_demo_window = true;   // DEBUG
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
}

static void game_refresher(void *data)
{
    GameState *game = (GameState *)data;
    while (42) {
        update_game_state(game);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(int, char**)
{
    GameState game;
    init_game_state(&game);

    std::thread game_thread(game_refresher, (void *)&game);
    game_thread.detach();

    if (!render_loop(frame_callback, (void *)&game)) {
        return EXIT_FAILURE;
    }
    // destroy_game_state(&game); //who cares?
    return EXIT_SUCCESS;
}
