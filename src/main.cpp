#include "smrfhck.hpp"

SDL_Window *window;
SDL_GLContext gl_context;

ImColor red(COLOR_RED_1, 0.8f);
ImColor green(COLOR_GREEN_1, 0.8f);
ImColor green_bis(COLOR_GREEN_2, 0.8f);
ImColor blue(COLOR_BLUE_1, 0.8f);
ImColor yellow(COLOR_YELLOW_1, 0.8f);
ImColor cyan(COLOR_CYAN_1, 0.8f);
ImColor magenta(COLOR_MAGENTA_1, 0.8f);
ImColor white(COLOR_WHITE_1, 0.8f);
ImColor black(COLOR_BLACK_1, 0.8f);

#define ABS(x) ((x) > 0 ? (x) : -(x))


inline static BOOL is_shrine(DWORD id)
{
    return id == 2   // Shrine
        || id == 77  // Healthorama
        || id == 81  // Forest Altar
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

inline static BOOL is_quest(DWORD id)
{
    return id == 8   // Tower Tome
        || id == 17  // StoneAlpha
        || id == 18  // StoneBeta
        || id == 19  // StoneGamma
        || id == 20  // StoneDelta
        || id == 21  // StoneLambda
        || id == 22  // StoneTheta
        || id == 30  // Inifuss Tree
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
        // || id == 523 //  Blacksmith // it's larzuk forge eh
        || id == 473 // Caged Fellow
        || id == 546 // AncientsAltar
        || id == 558 // Frozen Anya
        || id == 742; // tristram portal
}

inline static BOOL is_waypoint(DWORD id)
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

inline static BOOL is_transit(DWORD id)
{
    return id == 59  // Town Portal
        || id == 74  // Trap Door
        || id == 60  // Permanent Town Portal
        || id == 194 // Stairsl
        || id == 195 // Stairsr
        // || id == 244 // Sewers
        // || id == 246 // Sewers
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
        // || id == 189 // Cain Portal
        // || id == 268 // Wirt's Body
        // || id == 377 // Portal To Guild
        // || id == 451 // Hellgate
}

// static BOOL is_interesting_preset(DWORD id)
// {
//     return is_quest(id)
//         || is_shrine(id)
//         || is_waypoint(id)
//         || is_transit(id)
//         || id == 267; // Bank
// }

static BOOL is_uninteresting_unit(DWORD id)
{
    return id == 149 // rock
        || id == 152 // rogue
        || id == 179 // cow
        || id == 195 // lut gholein stuff
        || id == 196 // lut gholein tree
        || id == 197 // lut gholein stuff
        || id == 203 // lut gholein npc
        || id == 205 // lut gholein npc
        || id == 294 // kurast stuff
        || id == 296 // kurast stuff
        || id == 359 // iron wolf
        || id == 822 // fallen stuff
        || id == 823 // fallen stuff

        // || id == 513 // drehya
        // || id == 563 // baal tentacle
        // || id == 564 // baal tentacle
        // || id == 565 // baal tentacle
        // || id == 566 // baal tentacle
        || id == 567 // baal tentacle
        || id == 568 // injuried barb
        || id == 569 // injuried barb
        || id == 570 // injuried barb
        // || id == 571 // baal crab clone
        // || id == 575 // worldstone effect
        ;
}

static BOOL is_backward_tile(DWORD id)
{
    return id == 5 //Cave Down
        || id == 8 //Crypt Up
        || id == 11 //Tower to Wilderness
        || id == 13 //Jail Up
        || id == 16 //Catacombs to Cathedral
        || id == 17 //Catacombs Up
        || id == 21 //Sewer Dock to Town
        || id == 22 //Sewer Up
        || id == 25 //Harem to Town
        || id == 26 //Harem Up 1
        || id == 27 //Harem Up 2
        || id == 30 //Basement Up 1
        || id == 31 //Basement Up 2
        || id == 45 //Tomb Up
        || id == 48 //Lair Up
        || id == 52 //Spider to Jungle
        || id == 55 //Dungeon Up
        || id == 58 //Sewer Up L
        || id == 59 //Sewer Up R
        || id == 62 //Temple Up L
        || id == 63 //Temple Up R
        || id == 65 //Mephisto Up L
        || id == 66 //Mephisto Up R
        || id == 70 //Lava to Mesa
        || id == 73 //Ice Caves Up
        || id == 78 //Temple Up
        || id == 81; //Baal Temple Up
}

static DWORD act_from_area(DWORD area)
{
    if (area < 40) {
        return 1;
    } else if (area < 75) {
        return 2;
    } else if (area < 103) {
        return 3;
    } else if (area < 109) {
        return 4;
    } else {
        return 5;
    }
}

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

static void game_refresher()
{
    while (42) {
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

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

int main(int, char**)
{
    GameState *game = NULL;
    while (!(game = refresh())) {
        sleep(1);
    }

    std::thread game_thread(game_refresher);
    game_thread.detach();

    if (!render_loop(frame_callback, (void *)game)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
