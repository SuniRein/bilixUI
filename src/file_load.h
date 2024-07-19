#pragma once

#include <filesystem>

namespace bilixUI
{

const std::filesystem::path ASSET_PATH               = BILIXUI_ASSET_PATH;
const std::filesystem::path FONT_PATH                = ASSET_PATH / "font";

const std::filesystem::path DEFAULT_FONT             = FONT_PATH / BILIXUI_DEFAULT_FONT;
constexpr             float DEFAULT_FONT_SIZE        = 30;

void load_font(const std::filesystem::path& path);

} // namespace bilixUI
