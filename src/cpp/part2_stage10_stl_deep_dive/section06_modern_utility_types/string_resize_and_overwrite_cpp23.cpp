// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_resize_and_overwrite_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/string_resize_and_overwrite_cpp23
//
// Covers: string::resize_and_overwrite C++23 P1072

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <string>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string s;
    s.resize_and_overwrite(5, [](char* buf, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            buf[i] = static_cast<char>('a' + static_cast<int>(i));
        }
        return n;
    });
    assert(s == "abcde");
#else
    std::string s(5, '\0');
    for (std::size_t i = 0; i < s.size(); ++i) {
        s[i] = static_cast<char>('a' + static_cast<int>(i));
    }
    assert(s == "abcde");
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string s;
    s.resize_and_overwrite(10, [](char* buf, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            buf[i] = static_cast<char>('0' + (i % 10));
        }
        return std::size_t{6};  // actual length may be shorter
    });
    assert(s == "012345");
    assert(s.size() == 6);
#else
    std::string s = "012345";
    assert(s.size() == 6);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string s = "prefix";
    s.resize_and_overwrite(3, [](char* buf, std::size_t n) {
        // may shrink; buffer provided for overwrite
        if (n >= 3) {
            buf[0] = 'X';
            buf[1] = 'Y';
            buf[2] = 'Z';
            return std::size_t{3};
        }
        return n;
    });
    assert(s == "XYZ");
#else
    std::string s = "XYZ";
    assert(s == "XYZ");
#endif
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_resize_and_overwrite_cpp23", run>;

}  // namespace
