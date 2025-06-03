#include "smrfhck.hpp"

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

std::atomic_bool exit_now = false;

std::map<const char *, Setting> g_settings {
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
            .color=ImColor(COLOR_RED_1, 0.8f), .size=0.005f, .is_circle=1}},
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
            .color=ImColor(COLOR_WHITE_1, 0.1f), .size=0.025f, .is_circle=0}},
};  //TODO: add cool default colors / shape / size


static dword get_level_size(GameState *game)
{
    dword max = 0;
    Room2 *r2 = game->level->pRoom2First;
    while (r2) {
        max = MAX(max, (r2->dwPosX - game->level->dwPosX + r2->dwSizeX) * 5);
        max = MAX(max, (r2->dwPosY - game->level->dwPosY + r2->dwSizeY) * 5);
        r2 = r2->pNext;
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

        ImColor *color = &g_settings[lvl->dwLevelNo > game->level->dwLevelNo ?
                                     NEXT_LEVEL_SETTING_STR
                                     : PREV_LEVEL_SETTING_STR].color;
        if (lvl->pRoom2First) {
            for (Room2 *r = lvl->pRoom2First; r; r = r->pNext) {
                draw_rect(((float)r->dwPosX - (float)game->level->dwPosX) / max_size,
                          ((float)r->dwPosY - (float)game->level->dwPosY) / max_size,
                          (float)r->dwSizeX / max_size,
                          (float)r->dwSizeY / max_size,
                          color);
            }
        } else {
            // LOG_INFO("no room2 in lvl %d", lvl->dwLevelNo);
            draw_rect(((float)lvl->dwPosX - (float)game->level->dwPosX) / max_size,
                      ((float)lvl->dwPosY - (float)game->level->dwPosY) / max_size,
                      (float)lvl->dwSizeX / max_size,
                      (float)lvl->dwSizeY / max_size,
                      color); //level
        }

    }
}

static void draw_presets(Room2 *r2, Level *level, float max_size)
{
    Setting *setting;

    for (PresetUnit *pu = r2->pPreset; pu; pu = pu->pNext) {
        if (pu->dwType == UNIT_MONSTER) { //monster/npc
            if (is_uninteresting_unit(pu->dwTxtFileNo)) {
                continue;
            }
            setting = &g_settings[NPC_PRESET_SETTING_STR];
            // if (pu->dwTxtFileNo < 734) { //super unique boss
            //     LOG_INFO("preset: id=%d type=%d (%d, %d)",
            //              pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);
            // }
        } else if (pu->dwType == UNIT_OBJECT) {
            if (is_waypoint(pu->dwTxtFileNo)) {
                setting = &g_settings[WAYPOINT_SETTING_STR];
            } else if (is_quest(pu->dwTxtFileNo)) {
                setting = &g_settings[QUEST_SETTING_STR];
            } else if (is_shrine(pu->dwTxtFileNo)) {
                setting = &g_settings[SHRINE_SETTING_STR];
            } else if (is_transit(pu->dwTxtFileNo)) {
                setting = &g_settings[WEIRD_CONNECTION_SETTING_STR];
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                // continue;
                setting = &g_settings[BORING_SETTING_STR];
            }
        } else if (pu->dwType == UNIT_TILE) {
            if (is_backward_tile(pu->dwTxtFileNo)) { //probably not what you're searching for
                setting = &g_settings[LEVEL_CONNECTION_UP_SETTING_STR];
            } else {
                setting = &g_settings[LEVEL_CONNECTION_DOWN_SETTING_STR];
            }
        } else { // ???
            setting = &g_settings[UNKNOWN_SETTING_STR];
            LOG_INFO("preset: id=%d type=%d (%d, %d)",
                     pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);

        }
        draw(((float)r2->dwPosX - (float)level->dwPosX + (float)pu->dwPosX / 5.f) / max_size,
             ((float)r2->dwPosY - (float)level->dwPosY + (float)pu->dwPosY / 5.f) / max_size,
             setting);
    }

}

static bool draw_unit_callback(void *node_value, void *data)
{
    GameState *game = (GameState *)data;
    UnitAny *u = (UnitAny *)node_value;

    Setting *setting;

    if (!u->pPath) {
        // LOG_INFO("Unit without Path: id=%d type=%d txt=%d",
        //          u->dwUnitId, u->dwType, u->dwTxtFileNo);
        // log_UnitAny(u);
        return FALSE;
    }

    if (u->dwType == UNIT_PLAYER) {
        //TODO: handle players (and merc?)
        setting = &g_settings[OTHER_PLAYERS_SETTING_STR];

    } else if (u->dwType == UNIT_MONSTER) { //monster/npc
        byte type_flag = u->pMonsterData->fType;
        if (!type_flag) {
            setting = &g_settings[MONSTER_NORMAL_SETTING_STR];
        } else if (type_flag & MONSTER_SUPER) {
            if ((type_flag & MONSTER_SUPER) != MONSTER_SUPER)  {
                LOG_DEBUG("Unknown MONSTER_SUPER type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_SUPER_SETTING_STR];
        } else if (type_flag & MONSTER_CHAMP) {
            if ((type_flag & MONSTER_CHAMP) != MONSTER_CHAMP)  {
                LOG_DEBUG("Unknown MONSTER_CHAMP type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_CHAMP_SETTING_STR];
        } else if (type_flag & MONSTER_BOSS) {
            if ((type_flag & MONSTER_BOSS) != MONSTER_BOSS)  {
                LOG_DEBUG("Unknown MONSTER_BOSS type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_BOSS_SETTING_STR];
        } else if (type_flag & MONSTER_MINION) {
            if ((type_flag & MONSTER_MINION) != MONSTER_MINION)  {
                LOG_DEBUG("Unknown MONSTER_MINION type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_MINION_SETTING_STR];
        } else if (type_flag & (MONSTER_POSSESSED | MONSTER_GHOSTLY | MONSTER_MULTISHOT)) {
            setting = &g_settings[MONSTER_BOSS_SETTING_STR];
            LOG_DEBUG("Unknown MONSTER (possessed / ghostly / multishot) type flag: type=%d txt=%d flag=%02hhx",
                      u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
        // } else if (type_flag & MONSTER_NORMAL) {
        //     if ((type_flag & MONSTER_NORMAL) != MONSTER_NORMAL)  {
        //         LOG_DEBUG("Unknown MONSTER_NORMAL type flag: type=%d txt=%d flag=%02hhx",
        //                   u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
        //     }
        //     setting = &g_settings[MONSTER_NORMAL_SETTING_STR];
        } else {
            setting = &g_settings[MONSTER_NORMAL_SETTING_STR];
            // LOG_DEBUG("Unknown MONSTER type flag: id=%d type=%d txt=%d flag=%02hhx",
            //           u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
        }
        //TODO: handle boring monsters
    } else if (u->dwType == UNIT_OBJECT) {
        // TODO: chest / shrine
        return FALSE;
    } else {
        // setting = &g_settings[UNKNOWN_SETTING_STR];
        // LOG_INFO("Unknown Unit: id=%d type=%d txt=%d",
        //          u->dwUnitId, u->dwType, u->dwTxtFileNo);
        // log_UnitAny(u);
        return FALSE;
    }

    float max_size = (float)game->level->_2; // :|
    draw(((float)u->pPath->xPos / 5.f - (float)game->level->dwPosX) / max_size,
         ((float)u->pPath->yPos / 5.f - (float)game->level->dwPosY) / max_size,
         setting);

    return FALSE;
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

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pNext) {
        draw_rect(((float)r2->dwPosX - (float)game->level->dwPosX) / max_size,
                  ((float)r2->dwPosY - (float)game->level->dwPosY) / max_size,
                  (float)r2->dwSizeX / max_size,
                  (float)r2->dwSizeY / max_size,
                  r2->pRoom1 ? &g_settings[EXPLORED_AREAS_SETTING_STR].color
                  : &g_settings[CURRENT_LEVEL_SETTING_STR].color); //room2

        // for (Room1 *r1 = r2->pRoom1; r1; r1 = r1->pNext) {
        //     draw_rect((float)(r1->dwPosXBig / 5.f - game->level->dwPosX) / max_size,
        //               (float)(r1->dwPosYBig / 5.f - game->level->dwPosY) / max_size,
        //               (float)r1->dwSizeXBig / 5.f / max_size,
        //               (float)r1->dwSizeYBig / 5.f / max_size,
        //               &g_settings[EXPLORED_AREAS_SETTING_STR].color); //room1
        // }  // this doesn't show much actually
    }

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pNext) {
        //2nd iterate so it's drawn over room2
        draw_presets(r2, game->level, max_size);
    }

    game->level->_2 = (dword)max_size;  // erk, sorry
    hiter(g_unit_table, draw_unit_callback, game);

    draw(((float)game->player.pPath->xPos / 5.f - (float)game->level->dwPosX) / max_size,
         ((float)game->player.pPath->yPos / 5.f - (float)game->level->dwPosY) / max_size,
         &g_settings[PLAYER_SETTING_STR]); //player
}

////////////////////////////////////////////////////////////////////////////////

inline static float distance(float x1, float y1, float x2, float y2)
{
    return hypotf(x1 - x2, y1 - y2);
}

inline static void draw_debug(GameState *game)
{
    std::map<qword, PresetUnit *> preset_map;

    ImVec2 player((float)game->player.pPath->xPos / 5.f - (float)game->level->dwPosX,
                  (float)game->player.pPath->yPos / 5.f - (float)game->level->dwPosY);

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pNext) {
        for (PresetUnit *pu = r2->pPreset; pu; pu = pu->pNext) {
            ImVec2 preset((float)r2->dwPosX - (float)game->level->dwPosX + (float)pu->dwPosX / 5.f,
                          (float)r2->dwPosY - (float)game->level->dwPosY + (float)pu->dwPosY / 5.f);
            float pu_dist = distance(player.x, player.y, preset.x, preset.y);
            if (pu_dist > 1.5) {
                continue;
            }
            preset_map[((qword)pu->dwType << 32) + pu->dwTxtFileNo] = pu;
        }
    }

    ImGui::Text("Type Id");
    ImGui::SameLine(80);
    ImGui::Text("Category");
    for (auto it = preset_map.begin(); it != preset_map.end(); ++it) {
        PresetUnit *pu = it->second;

        ImGui::Text("%d    %d", pu->dwType, pu->dwTxtFileNo);
        ImGui::SameLine(80);

        if (pu->dwType == 1) { //monster/npc
            // if (is_uninteresting_unit(pu->dwTxtFileNo)) {
            //     continue;
            // }
            ImGui::Text(NPC_PRESET_SETTING_STR);
        } else if (pu->dwType == 2) {  //object
            if (is_waypoint(pu->dwTxtFileNo)) {
                ImGui::Text(WAYPOINT_SETTING_STR);
            } else if (is_quest(pu->dwTxtFileNo)) {
                ImGui::Text(QUEST_SETTING_STR);
            } else if (is_shrine(pu->dwTxtFileNo)) {
                ImGui::Text(SHRINE_SETTING_STR);
            } else if (is_transit(pu->dwTxtFileNo)) {
                ImGui::Text(WEIRD_CONNECTION_SETTING_STR);
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                // continue;
                ImGui::Text(BORING_SETTING_STR);
            }
        } else if (pu->dwType == 5) {  //tiles
            if (is_backward_tile(pu->dwTxtFileNo)) { //probably not what you're searching for
                ImGui::Text(LEVEL_CONNECTION_UP_SETTING_STR);
            } else {
                ImGui::Text(LEVEL_CONNECTION_DOWN_SETTING_STR);
            }
        } else { // ???
            ImGui::Text(UNKNOWN_SETTING_STR);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

inline static void draw_settings(GameState *game)
{
    // static bool check = true;
    int i = 0;

    ImGui::SameLine();
    ImGui::PushID(i++);
    ImGui::InputTextWithHint("", "Window Title",
                             game->window_title_setting,
                             PLAYER_DATA_NAME_MAX);
    ImGui::PopID();
    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::SameLine(10);
    ImGui::Text("Color");
    ImGui::SameLine(100);
    ImGui::Text("What");
    ImGui::SameLine(230);
    ImGui::Text("Size");
    ImGui::SameLine(280);
    ImGui::Text("Circle?");

    // ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
        ImGui::ColorEdit4(it->first,
                          (float *)&it->second.color,
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        ImGui::Text("%s", it->first);
        if (strcmp(it->first, CURRENT_LEVEL_SETTING_STR)
                && strcmp(it->first, PREV_LEVEL_SETTING_STR)
                && strcmp(it->first, NEXT_LEVEL_SETTING_STR)) { // those are just rect
            ImGui::SameLine(200);
            ImGui::PushItemWidth(90);
            ImGui::PushID(i++);
            ImGui::InputFloat("", &it->second.size, 0.001f, 0.1f, "%.3f");
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(i++);
            ImGui::Checkbox("", &it->second.is_circle);
            ImGui::PopID();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

inline static void frame_callback(void *data)
{
    GameState *game = (GameState *)data;

    ImGui::SetNextWindowPos(ImVec2(10, 25),
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH - 20, WINDOW_HEIGHT - 35),
                             ImGuiCond_FirstUseEver);
    if (ImGui::Begin("smrfhck")) {
        pthread_mutex_lock(&game->mutex);
        if (!game->level) {
            ImGui::Text("%s", *game->status ? game->status : "Loading...");
        } else {
            draw_map(game);
        }
        pthread_mutex_unlock(&game->mutex);
    }
    ImGui::End();

    static bool show_settings = true;
    if (show_settings) {
        ImGui::SetNextWindowPos(ImVec2(10, 3),
                                ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowCollapsed(TRUE, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("settings", &show_settings, 0)) {
            pthread_mutex_lock(&game->mutex);
            draw_settings(game);
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
    // }
}

static bool redirect_output_to_file(const char *filename)
{
    int fd = open(filename, O_CREAT | O_RDWR, 00644);
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

    while (!exit_now) {
        if (!update_game_state(game)) {
            std::this_thread::sleep_for(fail_delay);
        } else {
            std::this_thread::sleep_for(frame_delay);
        }
    }
    exit_now = false;
}

int main(int, char**)
{
    int ret = EXIT_SUCCESS;

    redirect_output_to_file(LOG_FILE);
    LOG_INFO("\n\n%s *Starting smrfhck*", get_date_str());

    GameState game;
    init_game_state(&game);

    std::thread game_thread(game_refresher, (void *)&game);
    game_thread.detach();

    if (!render_loop(frame_callback, (void *)&game)) {
        ret = EXIT_FAILURE;
    }

    exit_now = true;
#ifdef NDEBUG
    while (exit_now) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    destroy_game_state(&game);
    // this is basically to make sure we didn't leak any memory
#endif

    return ret;
}
