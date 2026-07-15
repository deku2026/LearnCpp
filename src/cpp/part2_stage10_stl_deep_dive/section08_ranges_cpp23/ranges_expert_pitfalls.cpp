// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_expert_pitfalls
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_expert_pitfalls
//
// Covers: ranges expert pitfalls: temporaries, dangling, O(n) size, multi-pass

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    // Temporary range materialization: bind to auto view carefully
    std::vector<int> v{1, 2, 3, 4};
    auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });
    // OK: v outlives evens
    LEARN_CHECK(std::ranges::distance(evens) == 2);
}

void demo_intermediate() {
    // filter_view is single-pass friendly but not sized
    auto f = std::views::iota(0, 10) | std::views::filter([](int x) { return x % 2 == 0; });
    static_assert(!std::ranges::sized_range<decltype(f)>);
    // distance is O(n)
    LEARN_CHECK(std::ranges::distance(f) == 5);
}

void demo_expert() {
    // dangling: algorithm on temporary non-borrowed range
    auto d = std::ranges::find(std::vector<int>{1, 2, 3}, 2);
    static_assert(std::same_as<decltype(d), std::ranges::dangling>);
    (void)d;

    // Multi-pass: prefer materialize when reusing expensive pipeline
    std::vector<std::string> ws{"a", "bb", "ccc"};
    auto pipe = ws | std::views::filter([](const std::string& s) { return s.size() > 1; }) |
                std::views::transform([](const std::string& s) { return s.size(); });
    std::vector<std::size_t> once(pipe.begin(), pipe.end());
    LEARN_CHECK(once.size() == 2);
    LEARN_CHECK(once[0] == 2 && once[1] == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_expert_pitfalls", run>;

}  // namespace
