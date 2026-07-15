// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_split
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_split
//
// Covers: views::split / lazy_split

#include "learn/topic_registry.hpp"

#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

void demo_basics() {
    std::string s = "a,b,c";
    auto parts = s | std::views::split(',');
    std::vector<std::string> out;
    for (auto p : parts) {
        out.emplace_back(p.begin(), p.end());
    }
    LEARN_CHECK((out == std::vector<std::string>{"a", "b", "c"}));
}

void demo_intermediate() {
    std::string_view sv = "x--y--z";
    auto parts = sv | std::views::split(std::string_view{"--"});
    int n = 0;
    for (auto p : parts) {
        (void)p;
        ++n;
    }
    LEARN_CHECK(n == 3);
}

void demo_expert() {
    std::string s = "one two three";
    auto parts = s | std::views::split(' ');
    auto it = parts.begin();
    std::string first((*it).begin(), (*it).end());
    LEARN_CHECK(first == "one");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_split", run>;

}  // namespace
