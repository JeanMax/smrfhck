#include "win/win.hpp"
#include "settings.hpp"


void draw_menu(GameState *game)
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
    for (Setting *set = g_settings; set - g_settings < MAX_SETTINGS; set++) {
        ImGui::ColorEdit4(set->name,
                          (float *)&set->color,
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        ImGui::Text("%s", set->name);
        SettingId id = (SettingId)(set - g_settings);
        if (id != EXPLORED_AREAS_SETTING && id != CURRENT_LEVEL_SETTING
            && id != NEXT_LEVEL_SETTING && id != PREV_LEVEL_SETTING) { // those are just rect
            ImGui::SameLine(200);
            ImGui::PushItemWidth(90);
            ImGui::PushID(i++);
            ImGui::InputFloat("", &set->size, 0.001f, 0.1f, "%.3f");
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(i++);
            ImGui::Checkbox("", &set->is_circle);
            ImGui::PopID();
        }
    }
}
