#include "file_load.h"

#include <filesystem>
#include <stdexcept>

#include "fmt/format.h"

#include "imgui.h"

namespace bilixUI
{

using namespace std::string_literals;

void load_font(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error(fmt::format("Unable to load font. Font path {} does not exist.", path.string()));
    }

    if (const auto& extansion = path.extension(); extansion != ".ttf" && extansion != ".otf")
    {
        throw std::runtime_error("Unsupported font file format. Only support ttf and otf file.");
    }

    ImGui::GetIO().Fonts->AddFontFromFileTTF(
        path.c_str(), DEFAULT_FONT_SIZE, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull()
    );
}

} // namespace bilixUI
