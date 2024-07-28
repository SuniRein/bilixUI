#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "boost/process/pipe.hpp"

namespace bilixUI
{

struct VAInfoUnit
{
    std::string quality;
    std::string codec;
    std::string size;
};

class VAInfo
{
  private:
    std::vector<VAInfoUnit> M_units;

  public:
    VAInfo() = default;

    void add(std::string_view quality, std::string_view codec, std::string_view size)
    {
        M_units.emplace_back(
            VAInfoUnit{
                std::string(quality), std::string(codec), std::string(size)
            }
        );
    }

    auto begin() const { return M_units.begin(); }
    auto end()   const { return M_units.end(); }
};

class MediaInfo
{
  private:
    std::string M_title;
    std::string M_count_watch, M_count_like, M_count_coin;

    VAInfo M_video_info;
    VAInfo M_audio_info;

  public:
    explicit MediaInfo(boost::process::ipstream& stream);

    std::string_view get_title()       const noexcept { return M_title; }
    auto             get_count_watch() const noexcept { return M_count_watch; }
    auto             get_count_like()  const noexcept { return M_count_like; }
    auto             get_count_coin()  const noexcept { return M_count_coin; }
    const auto&      get_video_info()  const noexcept { return M_video_info; }
    const auto&      get_audio_info()  const noexcept { return M_audio_info; }
};

MediaInfo get_media_info(std::string_view link);

} // namespace bilixUI
