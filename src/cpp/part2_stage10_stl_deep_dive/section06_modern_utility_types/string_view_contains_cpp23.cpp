// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_contains_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/string_view_contains_cpp23
//
// Covers: string_view::contains C++23

#include "learn/topic_registry.hpp"

#include <string_view>
#include <version>

namespace {

void demo_basics() {
    constexpr std::string_view sv = "hello world";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_CHECK(sv.contains("world"));
    LEARN_CHECK(sv.contains('o'));
    LEARN_CHECK(!sv.contains("xyz"));
#else
    LEARN_CHECK(sv.find("world") != std::string_view::npos);
    LEARN_CHECK(sv.find('o') != std::string_view::npos);
#endif
}

void demo_intermediate() {
    std::string_view sv = "abc";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_CHECK(sv.contains(std::string_view{"b"}));
#else
    LEARN_CHECK(sv.find("b") != std::string_view::npos);
#endif
}

void demo_expert() {
    constexpr std::string_view sv = "feature-test";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    static_assert(sv.contains("test"));
#endif
    LEARN_CHECK(sv.find("feature") == 0);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_view_contains_cpp23", run>;

}  // namespace
