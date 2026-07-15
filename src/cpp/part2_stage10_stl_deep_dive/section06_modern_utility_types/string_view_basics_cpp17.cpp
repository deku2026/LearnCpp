// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_basics_cpp17
// Topic id : part2/stage10/section06_modern_utility_types/string_view_basics_cpp17
//
// Covers: std::string_view non-owning string view zero-copy slice

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void process(std::string_view sv) {
    assert(!sv.empty() || sv.empty());
}

void demo_basics() {
    std::string s = "hello world";
    std::string_view sv = s;
    assert(sv.size() == 11);
    assert(sv.substr(0, 5) == "hello");
    process(s);
    process("literal");
}
void demo_intermediate() {
    std::string s = "hello world";
    std::string_view sv = s;
    sv.remove_prefix(6);
    assert(sv == "world");
    sv.remove_suffix(2);
    assert(sv == "wor");
    assert(sv.size() == 3);
}

void demo_expert() {
    constexpr std::string_view k = "compile-time";
    static_assert(k.size() == 12);
    static_assert(k.starts_with("comp"));
    static_assert(k.ends_with("time"));
    assert(k.find('t') != std::string_view::npos);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_view_basics_cpp17", run>;

}  // namespace
