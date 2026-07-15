// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_contains_cpp23
// Topic id : part3/section01/string_view_contains_cpp23
//
// Covers: std::string_view::contains C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string_view>
#include <version>

namespace {

void demo_basics() {
    std::string_view sv = "needle in haystack";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(sv.contains("needle"));
    assert(sv.contains('h'));
    assert(!sv.contains("xyz"));
#else
    assert(sv.find("needle") != std::string_view::npos);
#endif
}

void demo_intermediate() {
    std::string_view sv = "range";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(sv.contains(std::string_view{"ang"}));
#else
    assert(sv.find("ang") != std::string_view::npos);
#endif
}

void demo_expert() {
    std::string_view sv = "";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(sv.contains(""));
#else
    assert(sv.find("") == 0);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_contains_cpp23", run>;

}  // namespace
