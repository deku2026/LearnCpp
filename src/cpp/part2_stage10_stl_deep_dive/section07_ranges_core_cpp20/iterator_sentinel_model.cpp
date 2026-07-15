// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : iterator_sentinel_model
// Topic id : part2/stage10/section07_ranges_core_cpp20/iterator_sentinel_model
//
// Covers: iterator-sentinel separation in ranges

#include "learn/topic_registry.hpp"

#include <concepts>
#include <iterator>
#include <ranges>

namespace {

void demo_basics() {
    auto v = std::views::iota(0);
    auto it = v.begin();
    LEARN_CHECK(*it == 0);
    ++it;
    LEARN_CHECK(*it == 1);
    // infinite iota has unreachable sentinel; take bounds it
    auto t = v | std::views::take(3);
    LEARN_CHECK(std::ranges::distance(t) == 3);
}

void demo_intermediate() {
    auto v = std::views::iota(0, 5);
    auto b = v.begin();
    auto e = v.end();
    int n = 0;
    for (auto i = b; i != e; ++i) {
        ++n;
    }
    LEARN_CHECK(n == 5);
}

void demo_expert() {
    // sentinel may differ in type from iterator (not always same)
    auto r = std::views::iota(0, 3);
    using I = decltype(r.begin());
    using S = decltype(r.end());
    // equality i == s is supported even if types differ
    LEARN_CHECK(r.begin() != r.end());
    static_assert(std::sentinel_for<S, I> || std::same_as<I, S>);
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
    ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/iterator_sentinel_model", run>;

}  // namespace
