#include "gui.h"

#include "imgui.h"
#include "imgui_stdlib.h"

namespace bilixUI
{

bool draw(const char* name)
{
    static bool window_open = true;

    static bool first_init = true;
    static std::string link;
    static std::string output;

    if (first_init)
    {
        first_init = false;
        link.reserve(1000);
        output.reserve(2000);
    }

    ImGui::Begin(name, &window_open);
    {
        ImGui::InputText(u8"视频链接", &link);

        if (ImGui::Button(u8"获取视频信息"))
        {
            // TODO(SuniRein): Get video info.
        }

        if (ImGui::Button(u8"下载单个视频"))
        {
            // TODO(SuniRein): Download a single video.
        }

        if (ImGui::Button(u8"下载所有视频"))
        {
            // TODO(SuniRein): Download video list.
        }

        ImGui::TextUnformatted(output.c_str());
    }
    ImGui::End();

    return window_open;
}

} // namespace bilixUI
