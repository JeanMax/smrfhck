#include "smrfhck.hpp"

ImColor red(COLOR_RED_1, 0.8f);
ImColor green(COLOR_GREEN_1, 0.8f);
ImColor green_bis(COLOR_GREEN_2, 0.8f);
ImColor blue(COLOR_BLUE_1, 0.8f);
ImColor yellow(COLOR_YELLOW_1, 0.8f);
ImColor cyan(COLOR_CYAN_1, 0.8f);
ImColor magenta(COLOR_MAGENTA_1, 0.8f);
ImColor white(COLOR_WHITE_1, 0.8f);
ImColor black(COLOR_BLACK_1, 0.8f);

static DWORD get_level_size(GameState *game)
{
    DWORD max = 0;
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
    DWORD act = act_from_area(game->level->dwLevelNo);
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
                          &red);
            }
        } else {
            // LOG_INFO("no room2 in lvl %d", lvl->dwLevelNo);
            draw_rect(((float)lvl->dwPosX - (float)game->level->dwPosX) / max_size,
                      ((float)lvl->dwPosY - (float)game->level->dwPosY) / max_size,
                      (float)lvl->dwSizeX / max_size,
                      (float)lvl->dwSizeY / max_size,
                      &red); //level
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
            color = &red;
            // if (pu->dwTxtFileNo < 734) { //super unique boss
            //     LOG_INFO("preset: id=%d type=%d (%d, %d)",
            //              pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);
            // }
        } else if (pu->dwType == 2) {  //object
            if (is_waypoint(pu->dwTxtFileNo)) {
                color = &magenta;
            } else if (is_quest(pu->dwTxtFileNo)) {
                color = &blue;
            } else if (is_shrine(pu->dwTxtFileNo)) {
                color = &yellow;
            } else if (is_transit(pu->dwTxtFileNo)) {
                color = &cyan;
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                continue;
            }
        } else if (pu->dwType == 5) {  //tiles
            if (is_backward_tile(pu->dwTxtFileNo)) {
                color = &green_bis; //probably not what you're search for
            } else {
                color = &green;
            }
        } else { // ???
            color = &white;
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
                  &black); //room2



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
              &green); //player

}

////////////////////////////////////////////////////////////////////////////////

inline static void frame_callback(void *data)
{
    GameState *game = (GameState *)data;

    ImGui::Begin("smrfhck");
    pthread_mutex_lock(&game->mutex);

    if (!game->level) {
        ImGui::Text("Loading...");
    } else {
        draw_map(game);
    }

    pthread_mutex_unlock(&game->mutex);
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
