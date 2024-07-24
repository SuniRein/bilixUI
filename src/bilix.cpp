#include "bilix.h"

#include <iterator>
#include <string>

#include "boost/process/io.hpp"
#include "boost/process/pipe.hpp"
#include "boost/process/system.hpp"

namespace bilixUI
{

using namespace std::string_literals;

// TODO(SuniRein): Error handle for bilix.
std::string get_video_info(std::string_view link)
{
    boost::process::ipstream stream;

    boost::process::system(
        "bilix info "s + link.data(),
        boost::process::std_out > stream
    );

    return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

} // namespace bilixUI
