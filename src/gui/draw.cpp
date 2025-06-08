#include "gui/gui.hpp"

#define SQRT2  1.41421356f
#define COS_45 (SQRT2 / 2.f)
#define SIN_45 COS_45

void draw_circle(float x, float y, float radius, ImColor *color)
{
    //TODO: optimize
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

    // assume 0 >= x,y,radius >= 1, 1 == full width/height
    float max_size = MIN((vMax.x - vMin.x) / SQRT2, (vMax.y - vMin.y) / SQRT2);
    x *= max_size;
    y *= max_size;
    radius *= max_size;

    ImVec2 center(x, y);
    center = ImRotate(center, COS_45, SIN_45);

    //window offset
    center.x += vMin.x, center.y += vMin.y;

    //compensate rotate
    ImVec2 win_center = ImVec2((vMax.x - vMin.x) / 2, (vMax.y - vMin.y) / 2);
    center.x += win_center.x;

    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, *color, 0);
}

void draw_rect(float x, float y, float w, float h, ImColor *color)
{
    //TODO: optimize
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

    // assume 0 >= x,y,w,h >= 1, 1 == full width/height
    float max_size = MIN((vMax.x - vMin.x) / SQRT2, (vMax.y - vMin.y) / SQRT2);
    x *= max_size;
    y *= max_size;
    w *= max_size;
    h *= max_size;

    ImVec2 rect_a(x, y);
    ImVec2 rect_b(x + w, y);
    ImVec2 rect_c(x + w, y + h);
    ImVec2 rect_d(x, y + h);

    rect_a = ImRotate(rect_a, COS_45, SIN_45);
    rect_b = ImRotate(rect_b, COS_45, SIN_45);
    rect_c = ImRotate(rect_c, COS_45, SIN_45);
    rect_d = ImRotate(rect_d, COS_45, SIN_45);

    //window offset
    rect_a.x += vMin.x, rect_a.y += vMin.y;
    rect_b.x += vMin.x, rect_b.y += vMin.y;
    rect_c.x += vMin.x, rect_c.y += vMin.y;
    rect_d.x += vMin.x, rect_d.y += vMin.y;

    //compensate rotate
    ImVec2 win_center = ImVec2((vMax.x - vMin.x) / 2, (vMax.y - vMin.y) / 2);
    rect_a.x += win_center.x;
    rect_b.x += win_center.x;
    rect_c.x += win_center.x;
    rect_d.x += win_center.x;

    ImGui::GetWindowDrawList()->AddQuadFilled(rect_a,
                                              rect_b,
                                              rect_c,
                                              rect_d,
                                              *color);
}

void draw(float x, float y, Setting *s)
{
    if (s->is_circle) {
        draw_circle(x, y, s->size, &s->color);
    } else { //square
        draw_rect(x - s->size / 2, y - s->size / 2, s->size, s->size, &s->color);
    }
}
