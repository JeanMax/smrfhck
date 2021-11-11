#include "smrfhck.hpp"

#define PLAYER_COL_STR                "Player (it's you!)"
#define NPC_PRESET_COL_STR            "Npc/Monster Preset"
#define CURRENT_LEVEL_ROOM2_COL_STR   "Current Level's Room2"
#define OTHER_LEVEL_ROOM2_COL_STR     "Other Level's Room2"
#define OTHER_LEVEL_COL_STR           "Other Level"
#define LEVEL_CONNECTION_UP_COL_STR   "Level Connection Up"
#define LEVEL_CONNECTION_DOWN_COL_STR "Level Connection Down"
#define WEIRD_CONNECTION_COL_STR      "Weird Connection"
#define WAYPOINT_COL_STR              "Waypoint"
#define SHRINE_COL_STR                "Shrine"
#define QUEST_COL_STR                 "Quest"
#define UNKNOWN_COL_STR               "Unknown"
#define BORING_COL_STR                "Not Interesting"

std::map<const char *, ImColor> g_colors {
    {PLAYER_COL_STR,                ImColor(COLOR_GREEN_1, 0.8f)},
    {NPC_PRESET_COL_STR,            ImColor(COLOR_RED_1, 0.8f)},
    {CURRENT_LEVEL_ROOM2_COL_STR,   ImColor(COLOR_BLACK_1, 0.8f)},
    {OTHER_LEVEL_ROOM2_COL_STR,     ImColor(COLOR_RED_1, 0.8f)},
    {OTHER_LEVEL_COL_STR,           ImColor(COLOR_RED_1, 0.8f)},
    {LEVEL_CONNECTION_UP_COL_STR,   ImColor(COLOR_GREEN_2, 0.8f)},
    {LEVEL_CONNECTION_DOWN_COL_STR, ImColor(COLOR_GREEN_1, 0.8f)},
    {WEIRD_CONNECTION_COL_STR,      ImColor(COLOR_CYAN_1, 0.8f)},
    {WAYPOINT_COL_STR,              ImColor(COLOR_MAGENTA_1, 0.8f)},
    {SHRINE_COL_STR,                ImColor(COLOR_YELLOW_1, 0.8f)},
    {QUEST_COL_STR,                 ImColor(COLOR_BLUE_1, 0.8f)},
    {UNKNOWN_COL_STR,               ImColor(COLOR_WHITE_1, 0.8f)},
    {BORING_COL_STR,                ImColor(COLOR_WHITE_1, 0.0f)},
};


static dword get_level_size(GameState *game)
{
    dword max = 0;
                // draw_rect((float)(r2->dwPosX - game->level->dwPosX) * 5.f / (float)max_size,
                //           (float)(r2->dwPosY - game->level->dwPosY) * 5.f / (float)max_size,
                //           (float)r2->dwSizeX * 5.f / (float)max_size,
                //           (float)r2->dwSizeY * 5.f / (float)max_size,

    Room2 *r2 = game->level->pRoom2First;
    while (r2) {
        max = MAX(max, (r2->dwPosX - game->level->dwPosX + r2->dwSizeX) * 5 );
        max = MAX(max, (r2->dwPosY - game->level->dwPosY + r2->dwSizeY) * 5);
        r2 = r2->pRoom2Next;
    }
    return max;
}

static void draw_level_connection(GameState *game, float max_size)
{
    Level *lvl;
    // LOG_INFO("------------------------------------------------------------------");
    dword act = act_from_area(game->level->dwLevelNo);
    for (int i = 0; i < MAX_AREA; i++) {
        lvl = game->all_levels[i];
        if (!lvl
            || lvl->dwLevelNo == game->level->dwLevelNo
            || act_from_area(lvl->dwLevelNo) != act
            // || ABS((float)lvl->dwLevelNo - (float)game->level->dwLevelNo) > 2
            ) {
            continue;
        }

        if (lvl->pRoom2First) {
            for (Room2 *r = lvl->pRoom2First; r; r = r->pRoom2Next) {
                draw_rect(((float)r->dwPosX - (float)game->level->dwPosX) / max_size,
                          ((float)r->dwPosY - (float)game->level->dwPosY) / max_size,
                          (float)r->dwSizeX / max_size,
                          (float)r->dwSizeY / max_size,
                          &g_colors[OTHER_LEVEL_ROOM2_COL_STR]);
            }
        } else {
            // LOG_INFO("no room2 in lvl %d", lvl->dwLevelNo);
            draw_rect(((float)lvl->dwPosX - (float)game->level->dwPosX) / max_size,
                      ((float)lvl->dwPosY - (float)game->level->dwPosY) / max_size,
                      (float)lvl->dwSizeX / max_size,
                      (float)lvl->dwSizeY / max_size,
                      &g_colors[OTHER_LEVEL_COL_STR]); //level
        }

    }
}

static void draw_presets(Room2 *r2, Level *level, float max_size)
{
    ImColor *color = NULL;

    for (PresetUnit *pu = r2->pPreset; pu; pu = pu->pPresetNext) {
        if (pu->dwType == 1) { //monster/npc
            if (is_uninteresting_unit(pu->dwTxtFileNo)) {
                continue;
            }
            color = &g_colors[NPC_PRESET_COL_STR];
            // if (pu->dwTxtFileNo < 734) { //super unique boss
            //     LOG_INFO("preset: id=%d type=%d (%d, %d)",
            //              pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);
            // }
        } else if (pu->dwType == 2) {  //object
            if (is_waypoint(pu->dwTxtFileNo)) {
                color = &g_colors[WAYPOINT_COL_STR];
            } else if (is_quest(pu->dwTxtFileNo)) {
                color = &g_colors[QUEST_COL_STR];
            } else if (is_shrine(pu->dwTxtFileNo)) {
                color = &g_colors[SHRINE_COL_STR];
            } else if (is_transit(pu->dwTxtFileNo)) {
                color = &g_colors[WEIRD_CONNECTION_COL_STR];
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                // continue;
                color = &g_colors[BORING_COL_STR];
            }
        } else if (pu->dwType == 5) {  //tiles
            if (is_backward_tile(pu->dwTxtFileNo)) {
                color = &g_colors[LEVEL_CONNECTION_UP_COL_STR]; //probably not what you're searching for
            } else {
                color = &g_colors[LEVEL_CONNECTION_DOWN_COL_STR];
            }
        } else { // ???
            color = &g_colors[UNKNOWN_COL_STR];
            LOG_INFO("preset: id=%d type=%d (%d, %d)",
                     pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);

        }
        draw_rect(((float)r2->dwPosX - (float)level->dwPosX + (float)pu->dwPosX / 5.f) / max_size - 0.0125f,
                  ((float)r2->dwPosY - (float)level->dwPosY + (float)pu->dwPosY / 5.f) / max_size - 0.0125f,
                  0.025f,
                  0.025f,
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
                  &g_colors[CURRENT_LEVEL_ROOM2_COL_STR]); //room2



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

    draw_rect(((float)game->player.pPath->xPos / 5.f - (float)game->level->dwPosX) / max_size - 0.02f,
              ((float)game->player.pPath->yPos / 5.f - (float)game->level->dwPosY) / max_size - 0.02f,
              0.04f, 0.04f,
              &g_colors[PLAYER_COL_STR]); //player

}

////////////////////////////////////////////////////////////////////////////////


inline static void draw_settings(void)
{
    for (auto it = g_colors.begin(); it != g_colors.end(); ++it) {
        ImGui::ColorEdit4(it->first, (float*)&it->second, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        ImGui::Text("%s", it->first);
    }
}

////////////////////////////////////////////////////////////////////////////////

inline static void frame_callback(void *data)
{
    GameState *game = (GameState *)data;
    bool p_open;

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


    if (ImGui::Begin("settings", &p_open)) {
        draw_settings();
    }
    ImGui::End();
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
