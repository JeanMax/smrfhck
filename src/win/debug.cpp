#include <imgui_internal.h>  // hypotf

#include "win/win.hpp"
#include "settings.hpp"


inline static float distance(float x1, float y1, float x2, float y2)
{
    return hypotf(x1 - x2, y1 - y2);
}

void draw_debug(GameState *game)
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
