#pragma once

#include <regex>

#if __cplusplus >= 201703L
#include <string_view>
namespace sdptransform {
    using string_view=std::string_view;
    inline string_view convertMatchToView(const std::ssub_match& match)
    {
        #if __cplusplus >= 202002L && __cpp_lib_concepts
        return string_view(match.first, match.second);
        #else
        return string_view(&(*match.first), match.second-match.first);
        #endif
    }
}
#else
#include <string_view_lite.hpp>
namespace sdptransform {
    using string_view=string_view_lite;
    inline string_view convertMatchToView(const std::ssub_match& match)
    {
        return string_view(match.first, match.second);
    }
}
#endif
