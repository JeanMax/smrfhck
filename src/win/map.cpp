#include "win/win.hpp"
#include "gui/gui.hpp"  // draw, draw_rect
#include "settings.hpp"


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
    dword act = LEVEL_INFO[game->level->dwLevelNo].act;
    for (int i = 0; i < MAX_LEVEL; i++) {
        lvl = game->all_levels[i];
        if (!lvl
            || lvl->dwLevelNo == game->level->dwLevelNo
            || LEVEL_INFO[lvl->dwLevelNo].act != act) {
            continue;
        }

        ImColor *color = &g_settings[lvl->dwLevelNo > game->level->dwLevelNo ?
                                     NEXT_LEVEL_SETTING
                                     : PREV_LEVEL_SETTING].color;
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
            const MonsterInfo *info = NULL;
            const SuperInfo *super_info = NULL;

            if (pu->dwTxtFileNo >= MAX_MONSTER + MAX_SUPER) {
                setting = &g_settings[BOSS_ZONE_SETTING]; // boss / champ "zone"
                if (is_weird_preset_monster(pu->dwTxtFileNo)) {
                    continue;
                }
            } else if (pu->dwTxtFileNo >= MAX_MONSTER) {
                super_info = &SUPER_INFO[pu->dwTxtFileNo - MAX_MONSTER];
                info = &MONSTER_INFO[super_info->monster];
                setting = &g_settings[BOSS_ZONE_SETTING]; //super
            } else {
                info = &MONSTER_INFO[pu->dwTxtFileNo];
                setting = &g_settings[BOSS_ZONE_SETTING]; //normal (probably weird)
            }

            if (info && (!is_a_threat(info)
                         || is_npc(info)
                         || info->id == MON_SARCOPHAGUS
                         || info->id == MON_EVILHUT)) {
                continue;
            }

        } else if (pu->dwType == UNIT_OBJECT) {
            const ObjectInfo *info = &OBJECT_INFO[pu->dwTxtFileNo];

            if (is_waypoint(info)) {
                setting = &g_settings[WAYPOINT_SETTING];
            } else if (is_quest(info)) {
                setting = &g_settings[QUEST_SETTING]; //TODO: use another color for chest (non quest ones...)
            } else if (is_shrine(info)) {
                setting = &g_settings[SHRINE_SETTING];  //TODO: some shrines don't have preset :/
            } else if (is_portal(info)) {
                setting = &g_settings[ENTRANCE_DOWN_SETTING];
            } else if (is_transit(info)) {
                LOG_ERROR("This shouldn't happen: is_transit()");
                setting = &g_settings[ENTRANCE_UP_SETTING];
            } else {
                // continue;
                setting = &g_settings[BORING_SETTING];
            }

        } else if (pu->dwType == UNIT_TILE) {
            const TileInfo *_info = &TILE_INFO[pu->dwTxtFileNo];
            const UniqueTileInfo *info = &UNIQUE_TILE_INFO[_info->uid];

            if (is_backward_tile(info)) { //probably not what you're searching for
                setting = &g_settings[ENTRANCE_UP_SETTING];
            } else {
                setting = &g_settings[ENTRANCE_DOWN_SETTING];
            }

        } else { // ???
            setting = &g_settings[BORING_SETTING];
            LOG_WARNING("Unknown preset: id=%d type=%d (%d, %d)",
                        pu->dwTxtFileNo, pu->dwType, pu->dwPosX, pu->dwPosY);

        }

        draw(((float)r2->dwPosX - (float)level->dwPosX + (float)pu->dwPosX / 5.f) / max_size,
             ((float)r2->dwPosY - (float)level->dwPosY + (float)pu->dwPosY / 5.f) / max_size,
             setting);
        //TODO: for objects, you actually have their sizes
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

    if (u->wIsCorpse == 1) {
        return FALSE; //dead
    }

    if (u->dwType == UNIT_PLAYER) {
        setting = &g_settings[OTHER_PLAYERS_SETTING];

    } else if (u->dwType == UNIT_MONSTER) { //monster/npc
        const MonsterInfo *info = NULL;
        const SuperInfo *super_info = NULL;

        //TODO: add a fun / macro to handle this
        if (u->dwTxtFileNo >= MAX_MONSTER + MAX_SUPER) {
             // boss / champ "zone"
        } else if (u->dwTxtFileNo >= MAX_MONSTER) {
            super_info = &SUPER_INFO[u->dwTxtFileNo - MAX_MONSTER];
            info = &MONSTER_INFO[super_info->monster];
        } else {
            info = &MONSTER_INFO[u->dwTxtFileNo];
        }

        byte type_flag = u->pMonsterData->fType;
        if (info && is_npc(info)) {
            setting = &g_settings[MONSTER_NORMAL_SETTING];
        } else if (info && is_ally(info)) {
            setting = &g_settings[OTHER_PLAYERS_SETTING];
        } else if (info && !is_a_threat(info)) {
            setting = &g_settings[BORING_SETTING];
        } else if (!type_flag) {
            setting = &g_settings[MONSTER_NORMAL_SETTING];
        } else if (type_flag & MONSTER_SUPER) {
            if ((type_flag & MONSTER_SUPER) != MONSTER_SUPER)  {
                LOG_DEBUG("Unknown MONSTER_SUPER type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_SUPER_SETTING];
        } else if (type_flag & MONSTER_CHAMP) {
            if ((type_flag & MONSTER_CHAMP) != MONSTER_CHAMP)  {
                LOG_DEBUG("Unknown MONSTER_CHAMP type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_CHAMP_SETTING];
        } else if (type_flag & MONSTER_BOSS) {
            if ((type_flag & MONSTER_BOSS) != MONSTER_BOSS)  {
                LOG_DEBUG("Unknown MONSTER_BOSS type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_BOSS_SETTING];
        } else if (type_flag & MONSTER_MINION) {
            if ((type_flag & MONSTER_MINION) != MONSTER_MINION)  {
                LOG_DEBUG("Unknown MONSTER_MINION type flag: type=%d txt=%d flag=%02hhx",
                          u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
            }
            setting = &g_settings[MONSTER_MINION_SETTING];
        } else if (type_flag & (MONSTER_POSSESSED | MONSTER_GHOSTLY | MONSTER_MULTISHOT)) {
            setting = &g_settings[MONSTER_BOSS_SETTING];
            LOG_DEBUG("Unknown MONSTER (possessed / ghostly / multishot) type flag: type=%d txt=%d flag=%02hhx",
                      u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
        // } else if (type_flag & MONSTER_NORMAL) {
        //     if ((type_flag & MONSTER_NORMAL) != MONSTER_NORMAL)  {
        //         LOG_DEBUG("Unknown MONSTER_NORMAL type flag: type=%d txt=%d flag=%02hhx",
        //                   u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
        //     }
        //     setting = &g_settings[MONSTER_NORMAL_SETTING];
        } else {
            setting = &g_settings[MONSTER_NORMAL_SETTING];
            // LOG_DEBUG("Unknown MONSTER type flag: id=%d type=%d txt=%d flag=%02hhx",
            //           u->dwUnitId, u->dwType, u->dwTxtFileNo, type_flag);
        }

    } else if (u->dwType == UNIT_OBJECT) {
        // TODO: chest / shrine
        return FALSE;

    } else if (u->dwType == UNIT_MISSILE) {
        setting = &g_settings[MISSILE_SETTING];

    } else {
        // setting = &g_settings[BORING_SETTING];
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

void draw_map(GameState *game)
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
                  r2->pRoom1 ? &g_settings[EXPLORED_AREAS_SETTING].color
                  : &g_settings[CURRENT_LEVEL_SETTING].color); //room2

        // for (Room1 *r1 = r2->pRoom1; r1; r1 = r1->pNext) {
        //     draw_rect((float)(r1->dwPosXBig / 5.f - game->level->dwPosX) / max_size,
        //               (float)(r1->dwPosYBig / 5.f - game->level->dwPosY) / max_size,
        //               (float)r1->dwSizeXBig / 5.f / max_size,
        //               (float)r1->dwSizeYBig / 5.f / max_size,
        //               &g_settings[EXPLORED_AREAS_SETTING].color); //room1
        // }  // this doesn't show much actually
    }

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pNext) {
        //2nd iterate so it's drawn over room2
        draw_presets(r2, game->level, max_size);
    }

    game->level->_2 = (dword)max_size;  // erk, sorry
    hiter(g_unit_table, draw_unit_callback, game);

    draw(((float)game->player->pPath->xPos / 5.f - (float)game->level->dwPosX) / max_size,
         ((float)game->player->pPath->yPos / 5.f - (float)game->level->dwPosY) / max_size,
         &g_settings[PLAYER_SETTING]); //player
}
