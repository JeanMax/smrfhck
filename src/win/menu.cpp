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
    for (auto it = g_settings.begin(); it != g_settings.end(); ++it) {
        ImGui::ColorEdit4(it->first.c_str(),
                          (float *)&it->second.color,
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        ImGui::Text("%s", it->first.c_str());
        if (strcmp(it->first.c_str(), CURRENT_LEVEL_SETTING_STR)
                && strcmp(it->first.c_str(), PREV_LEVEL_SETTING_STR)
                && strcmp(it->first.c_str(), NEXT_LEVEL_SETTING_STR)) { // those are just rect
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
