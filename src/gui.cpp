#include "gui.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "bilix.h"

#include <sstream>

namespace bilixUI
{

bool draw(const char* name)
{
    static bool window_open = true;

    static bool first_init = true;
    static std::string link;
    static std::stringstream output;

    if (first_init)
    {
        first_init = false;
        link.reserve(1000);
    }

    ImGui::Begin(name, &window_open);
    {
        ImGui::InputText(u8"视频链接", &link);

        if (ImGui::Button(u8"获取视频信息"))
        {
            auto media_info = get_media_info(link);
            output.str("");
            output << u8"标题: "   << media_info.get_title()       << "\n";
            output << u8"观看数: " << media_info.get_count_watch() << "\n";
            output << u8"点赞数: " << media_info.get_count_like()  << "\n";
            output << u8"投币数: " << media_info.get_count_coin()  << "\n";

            for (const auto& [quality, codec, size] : media_info.get_video_info())
            {
                output << "quality: " << quality << " ";
                output << "codec: "   << codec   << " ";
                output << "size: "    << size    << "\n";
            }
            for (const auto& [quality, codec, size] : media_info.get_audio_info())
            {
                output << "quality: " << quality << " ";
                output << "codec: "   << codec   << " ";
                output << "size: "    << size    << "\n";
            }
        }

        if (ImGui::Button(u8"下载单个视频"))
        {
            // TODO(SuniRein): Download a single video.
        }

        if (ImGui::Button(u8"下载所有视频"))
        {
            // TODO(SuniRein): Download video list.
        }

        ImGui::TextUnformatted(output.str().c_str());
    }
    ImGui::End();

    return window_open;
}

} // namespace bilixUI
