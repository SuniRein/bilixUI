#pragma once


namespace bilixUI::config
{

#ifdef BILIXUI_DEBUG
    constexpr bool debug = true;
#else
    constexpr bool debug = false;
#endif

} // namespace bilixUI::config
