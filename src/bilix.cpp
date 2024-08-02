#include "bilix.h"

#include <string>

#include "boost/algorithm/string/predicate.hpp"
#include "boost/process/io.hpp"
#include "boost/process/pipe.hpp"
#include "boost/process/system.hpp"

#include "fmt/format.h"

namespace bilixUI
{

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
    // TODO(SuniRein): Error handle for bilix.
    auto get_raw_media_info(std::string_view link)
    {
        boost::process::ipstream stream;

        boost::process::system(
            fmt::format("bilix info {}", link),
            boost::process::std_out > stream
        );

        return stream;
    }

    std::string substring(std::string_view str, std::size_t begin, std::size_t end)
    {
        return std::string(str.substr(begin, end - begin));
    }
} // anonymous namespace

MediaInfo::MediaInfo(boost::process::ipstream& stream)
{
    constexpr auto TITLE_END = "-  "sv;
    constexpr auto WATCH_END = "\U0001F440 "sv;
    constexpr auto LIKE_END  = "\U0001F44D "sv;
    constexpr auto COIN_END  = "\U0001FA99"sv;

    constexpr std::string_view LEVEL_BEGINS[] = {
        "\U00002523\U00002501\U00002501 "sv, // "┣━━ "
        "\U00002503   "sv,                   // "┃   "
        "\U00002517\U00002501\U00002501 "sv, // "┗━━ "
        "    "sv,                            // "    "
    };

    using Level = std::size_t;
    constexpr Level LEVEL_NONE    = 0;
    constexpr Level LEVEL_VA      = 1;
    constexpr Level LEVEL_QUALITY = 2;
    constexpr Level LEVEL_DETAIL  = 3;

    const auto remove_line_level = [LEVEL_BEGINS](std::string_view str)
    {
        Level line_level = LEVEL_NONE;

        bool level_up = true;
        while (level_up)
        {
            level_up = false;

            for (auto level_begin : LEVEL_BEGINS)
            {
                if (boost::starts_with(str, level_begin))
                {
                    str = str.substr(level_begin.size());
                    ++line_level;
                    level_up = true;
                }
            }
        }

        return std::pair{line_level, str};
    };

    std::string str;

    // Get the title line.
    // format: " [title]x[count_watch]x[count_like]x[count_coin]x"
    if (std::getline(stream, str) && !str.empty())
    {
        const auto title_end_pos = str.find(TITLE_END);
        const auto watch_end_pos = str.find(WATCH_END);
        const auto like_end_pos  = str.find(LIKE_END);
        const auto coin_end_pos  = str.find(COIN_END);

        this->M_title       = substring(str, 1, title_end_pos);
        this->M_count_watch = substring(str, title_end_pos + TITLE_END.size(), watch_end_pos);
        this->M_count_like  = substring(str, watch_end_pos + WATCH_END.size(), like_end_pos);
        this->M_count_coin  = substring(str, like_end_pos  + LIKE_END .size(), coin_end_pos);
    }

    std::string quality, codec, size;
    bool is_video = false;

    while (stream && std::getline(stream, str) && !str.empty())
    {
        switch (auto [line_level, valid_str] = remove_line_level(str); line_level)
        {
            case LEVEL_VA:
            {
                is_video = boost::contains(valid_str, "Video");
                break;
            }
            case LEVEL_QUALITY:
            {
                quality = valid_str;
                break;
            }
            case LEVEL_DETAIL:
            {
                constexpr auto PERMISSION_DENIED = u8"需要登录或大会员"sv;
                if (valid_str == PERMISSION_DENIED)
                {
                    codec = PERMISSION_DENIED;
                    size  = PERMISSION_DENIED;
                }
                else
                {
                    constexpr auto CODEC_BEGIN = "codec: "sv;
                    constexpr auto SIZE_BEGIN  = "size: "sv;

                    const auto codec_begin = valid_str.find(CODEC_BEGIN);
                    const auto size_begin  = valid_str.find(SIZE_BEGIN);

                    codec = boost::trim_right_copy(substring(valid_str, codec_begin + CODEC_BEGIN.size(), size_begin));
                    size  = boost::trim_right_copy(substring(valid_str, size_begin  + SIZE_BEGIN.size(),  valid_str.size()));
                }

                if (is_video)
                {
                    this->M_video_info.add(quality, codec, size);
                }
                else
                {
                    this->M_audio_info.add(quality, codec, size);
                }

                break;
            }

            default: /*TODO(SuniRein): Eroor Handle*/;
        };
    }
}

MediaInfo get_media_info(std::string_view link)
{
    auto raw_media_info = get_raw_media_info(link);
    return MediaInfo(raw_media_info);
}

// TODO(SuniRein): Process invalid input.
std::string download_single_video(std::string_view link, const VAInfoUnit* video_unit, const VAInfoUnit* audio_unit)
{
    [[maybe_unused]] const auto [v_quality, v_codec, v_size] = video_unit ? *video_unit : VAInfoUnit{};
    [[maybe_unused]] const auto [a_quality, a_codec, a_size] = audio_unit ? *audio_unit : VAInfoUnit{};

    const std::string command = fmt::format(
        R"(bilix get_video --dir bilix {} --codec "{}:{}" --quality "{}")",
        link, v_codec, a_codec, v_quality
    );

    boost::process::system(command);

    return command;
}

void clear_downloaded_video()
{
    boost::process::system(
        "rm -rf bilix"
    );
}

} // namespace bilixUI
