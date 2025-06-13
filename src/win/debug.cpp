#include <imgui_internal.h> // hypotf
#include <map>

#include "win/win.hpp"
#include "settings.hpp"

#define CATEGORY_OFFSET 80
#define NAME_OFFSET 220

// TODO: ideally you'd want to have a fun that you would call from these debug
// fun and map.cpp so you wouldn't have to do it twice...
// category_of_preset() / category_of_unit() ?


inline static float distance(float x1, float y1, float x2, float y2)
{
    return hypotf(x1 - x2, y1 - y2);
}

static bool draw_unit_debug_callback(void *node_value, void *data)
{
    GameState *game = (GameState *)data;
    UnitAny *u = (UnitAny *)node_value;

    ImVec2 player((float)game->player.pPath->xPos,
                  (float)game->player.pPath->yPos);

    ImVec2 unit((float)u->pPath->xPos,
                (float)u->pPath->yPos);

    float u_dist = distance(player.x, player.y, unit.x, unit.y);

    if (u_dist > 10) {
        return FALSE;
    }

    ImGui::Text("%d    %d", u->dwType, u->dwTxtFileNo);
    ImGui::SameLine(CATEGORY_OFFSET);

    if (u->dwType == UNIT_PLAYER) {
        ImGui::Text(OTHER_PLAYERS_SETTING_STR);
        ImGui::SameLine(NAME_OFFSET);
        ImGui::Text("[%s]", u->pPlayerData->szName);
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
        if (!u->pPath) {
            ImGui::Text("!!!GHOST!!!");
        } else if (u->wIsCorpse == 1) {
            ImGui::Text("*DEAD*");
        } else if (info && is_npc(info)) {
            ImGui::Text(MONSTER_NORMAL_SETTING_STR);
        } else if (info && is_ally(info)) {
            ImGui::Text(OTHER_PLAYERS_SETTING_STR);
        } else if (info && !is_a_threat(info)) {
            ImGui::Text(BORING_SETTING_STR);
        } else if (!type_flag) {
            ImGui::Text(MONSTER_NORMAL_SETTING_STR);
        } else if (type_flag & MONSTER_SUPER) {
            ImGui::Text(MONSTER_SUPER_SETTING_STR);
        } else if (type_flag & MONSTER_CHAMP) {
            ImGui::Text(MONSTER_CHAMP_SETTING_STR);
        } else if (type_flag & MONSTER_BOSS) {
            ImGui::Text(MONSTER_BOSS_SETTING_STR);
        } else if (type_flag & MONSTER_MINION) {
            ImGui::Text(MONSTER_MINION_SETTING_STR);
        } else if (type_flag & (MONSTER_POSSESSED | MONSTER_GHOSTLY | MONSTER_MULTISHOT)) {
            ImGui::Text(MONSTER_BOSS_SETTING_STR);
        } else {
            ImGui::Text(MONSTER_NORMAL_SETTING_STR);
        }
        ImGui::SameLine(NAME_OFFSET);
        if (super_info) {
            ImGui::Text("[%s] %s (%s)", super_info->name, info->name, info->classId);
        } else if (info) {
            ImGui::Text("%s (%s)", info->name, info->classId);
        } else {
            ImGui::Text("{dunno}");
        }
    } else if (u->dwType == UNIT_OBJECT) {
        const ObjectInfo *info = &OBJECT_INFO[u->dwTxtFileNo];
        // TODO: chest / shrine
        ImGui::Text("Object: TODO");
        ImGui::SameLine(NAME_OFFSET);
        ImGui::Text("%s (%s: %s)", info->name, info->classId, info->desc);
    } else {
        ImGui::Text("n/a");
        ImGui::SameLine(NAME_OFFSET);
        ImGui::Text("{dunno}");
    }

    return FALSE;
}

static void draw_preset_debug(GameState *game)
{
    std::map<qword, PresetUnit *> preset_map;

    ImVec2 player((float)game->player.pPath->xPos / 5.f - (float)game->level->dwPosX,
                  (float)game->player.pPath->yPos / 5.f - (float)game->level->dwPosY);

    for (Room2 *r2 = game->level->pRoom2First; r2; r2 = r2->pNext) {
        for (PresetUnit *pu = r2->pPreset; pu; pu = pu->pNext) {
            ImVec2 preset((float)r2->dwPosX - (float)game->level->dwPosX + (float)pu->dwPosX / 5.f,
                          (float)r2->dwPosY - (float)game->level->dwPosY + (float)pu->dwPosY / 5.f);
            float pu_dist = distance(player.x, player.y, preset.x, preset.y);
            if (pu_dist > 2) {
                continue;
            }
            preset_map[((qword)pu->dwType << 32) + pu->dwTxtFileNo] = pu;
        }
    }

    for (auto it = preset_map.begin(); it != preset_map.end(); ++it) {
        PresetUnit *pu = it->second;

        ImGui::Text("%d    %d", pu->dwType, pu->dwTxtFileNo);
        ImGui::SameLine(CATEGORY_OFFSET);

        if (pu->dwType == UNIT_MONSTER) { //monster/npc
            const MonsterInfo *info = NULL;
            const SuperInfo *super_info = NULL;

            if (pu->dwTxtFileNo >= MAX_MONSTER + MAX_SUPER) {
              ImGui::Text(BOSS_ZONE_SETTING_STR " (weird)"); // boss / champ "zone"
            } else if (pu->dwTxtFileNo >= MAX_MONSTER) {
                super_info = &SUPER_INFO[pu->dwTxtFileNo - MAX_MONSTER];
                info = &MONSTER_INFO[super_info->monster];
                ImGui::Text(BOSS_ZONE_SETTING_STR " (super)"); //super
            } else {
                info = &MONSTER_INFO[pu->dwTxtFileNo];
                ImGui::Text(BOSS_ZONE_SETTING_STR); //normal (probably weird)
            }

            ImGui::SameLine(NAME_OFFSET);
            if (is_weird_preset_monster(pu->dwTxtFileNo)) {
                ImGui::Text("{weird}");
            } else if (super_info) {
                ImGui::Text("[%s] %s (%s)", super_info->name, info->name, info->classId);
            } else if (info) {
                ImGui::Text("%s (%s)", info->name, info->classId);
            } else {
                ImGui::Text("{dunno}");
            }

        } else if (pu->dwType == UNIT_OBJECT) {
            const ObjectInfo *info = &OBJECT_INFO[pu->dwTxtFileNo];

            if (is_waypoint(info)) {
                ImGui::Text(WAYPOINT_SETTING_STR);
            } else if (is_quest(info)) {
                ImGui::Text(QUEST_SETTING_STR);
            } else if (is_shrine(info)) {
                ImGui::Text(SHRINE_SETTING_STR);
            } else if (is_portal(info)) {
                ImGui::Text("Portal (" QUEST_SETTING_STR ")");
            } else if (is_transit(info)) {
                ImGui::Text("TRANSIT: shouldn't happen");
            } else { //!is_interesting_preset(pu->dwTxtFileNo)
                // continue;
                ImGui::Text(BORING_SETTING_STR);
            }

            ImGui::SameLine(NAME_OFFSET);
            ImGui::Text("%s (%s: %s)", info->name, info->classId, info->desc);

        } else if (pu->dwType == UNIT_TILE) {
            const TileInfo *_info = &TILE_INFO[pu->dwTxtFileNo];
            const UniqueTileInfo *info = &UNIQUE_TILE_INFO[_info->uid];

            if (is_backward_tile(info)) { //probably not what you're searching for
                ImGui::Text(ENTRANCE_UP_SETTING_STR);
            } else {
                ImGui::Text(ENTRANCE_DOWN_SETTING_STR);
            }

            ImGui::SameLine(NAME_OFFSET);
            ImGui::Text("%s (%d)", info->name, info->notUniqueId);
        } else { // ???
            ImGui::Text("DUNNO");
        }
    }
}

static void draw_debug_header()
{
    ImGui::Text("Type Id");
    ImGui::SameLine(CATEGORY_OFFSET);
    ImGui::Text("Category");
    ImGui::SameLine(NAME_OFFSET);
    ImGui::Text("Name");
}

void draw_debug(GameState *game)
{
    draw_debug_header();
    ImGui::Text("------------------------ Presets ----------------------");
    draw_preset_debug(game);
    ImGui::Text("\n\n------------------------- Units -----------------------");
    draw_debug_header();
    hiter(g_unit_table, draw_unit_debug_callback, game);
}
