#include "gui.h"

#include "imgui.h"

bool GUI_draw(const char* name)
{
    static bool window_open = true;

    ImGui::Begin(name, &window_open);
    ImGui::End();

    return window_open;
}
