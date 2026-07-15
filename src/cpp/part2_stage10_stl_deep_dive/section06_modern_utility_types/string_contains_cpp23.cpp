// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_contains_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/string_contains_cpp23
//
// Covers: std::string::contains C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

namespace {

void demo_basics() {
    std::string s = "hello world";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_CHECK(s.contains("world"));
    LEARN_CHECK(s.contains('h'));
    LEARN_CHECK(!s.contains("xyz"));
#else
    LEARN_CHECK(s.find("world") != std::string::npos);
#endif
}

void demo_intermediate() {
    std::string s = "C++23";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_CHECK(s.contains(std::string{"++"}));
#else
    LEARN_CHECK(s.find("++") != std::string::npos);
#endif
}

void demo_expert() {
    std::string s = "feature";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_CHECK(s.contains(std::string_view{"eat"}));
#else
    LEARN_CHECK(s.find("eat") != std::string::npos);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_contains_cpp23", run>;

}  // namespace
