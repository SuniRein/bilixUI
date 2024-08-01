#include "gui.h"

#include <optional>
#include <vector>
#include <string>

#include "fmt/format.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "config.h"
#include "bilix.h"

namespace bilixUI
{

using namespace std::string_literals;

namespace
{
    class VAInfoCombo
    {
      private:
        const VAInfo*            M_info = nullptr;
        std::vector<std::string> M_info_str_list;
        const VAInfoUnit*        M_selected_unit = nullptr;
        std::size_t              M_selected_unit_id = -1;

      public:
        void reset(const VAInfo& info)
        {
            M_info = &info;
            M_info_str_list.clear();
            M_selected_unit = nullptr;
            M_selected_unit_id = -1;

            for (const auto& [quality, codec, size] : info)
            {
                M_info_str_list.emplace_back(
                    fmt::format("{} | codec: {} | size: {}", quality, codec, size)
                );
            }
        }

        void show(const char* label)
        {
            const char* preview_value = M_selected_unit ? M_info_str_list[M_selected_unit_id].c_str() : "Default";
            if (ImGui::BeginCombo(label, preview_value, ImGuiComboFlags_HeightLargest))
            {
                if (M_info)
                {
                    for (const auto& info_unit : *M_info)
                    {
                        const std::size_t id = &info_unit - M_info->begin().base();
                        const bool is_selected = (M_selected_unit == &info_unit);
                        if (ImGui::Selectable(M_info_str_list[id].c_str(), is_selected))
                        {
                            M_selected_unit = &info_unit;
                            M_selected_unit_id = id;
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }
    };
} // anonymous namespace

bool draw(const char* name)
{
    static bool window_open = true;

    static bool first_init = true;
    static std::string link;
    static std::string output;

    static std::optional<const MediaInfo> media_info;

    static VAInfoCombo video_info_combo;
    static VAInfoCombo audio_info_combo;

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
            media_info.emplace(get_media_info(link));

            video_info_combo.reset(media_info->get_video_info());
            audio_info_combo.reset(media_info->get_audio_info());

            output = fmt::format(
                u8"标题: {}\n"
                u8"观看数: {}\n"
                u8"点赞数: {}\n"
                u8"投币数: {}\n",
                media_info->get_title(), media_info->get_count_watch(),
                media_info->get_count_like(), media_info->get_count_coin()
            );
        }

        if (ImGui::Button(u8"下载单个视频"))
        {
            // TODO(SuniRein): Download a single video.
        }

        if (ImGui::Button(u8"下载所有视频"))
        {
            // TODO(SuniRein): Download video list.
        }

        if constexpr (bilixUI::config::debug)
        {
            if (ImGui::Button(u8"示例链接1"))
            {
                link = "https://www.bilibili.com/video/BV1DW421X7zK/";
            }
            ImGui::SameLine();
            if (ImGui::Button(u8"示例链接2"))
            {
                link = "https://www.bilibili.com/video/BV1Ry411e7Ac/";
            }
        }

        ImGui::TextUnformatted(output.c_str());

        video_info_combo.show(u8"选择视频");
        audio_info_combo.show(u8"选择音频");
    }
    ImGui::End();

    return window_open;
}

} // namespace bilixUI
