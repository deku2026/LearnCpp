// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_apply
// Topic id : part2/stage10/section06_modern_utility_types/std_apply
//
// Covers: std::apply invoke with tuple args

#include "learn/topic_registry.hpp"

#include <cassert>
#include <tuple>
#include <utility>

namespace {

int add(int a, int b) {
    return a + b;
}

void demo_basics() {
    assert(std::apply(add, std::make_tuple(2, 3)) == 5);
}

void demo_intermediate() {
    auto t = std::make_tuple(10, 20, 30);
    auto sum = std::apply([](int a, int b, int c) { return a + b + c; }, t);
    assert(sum == 60);
}

void demo_expert() {
    std::pair<int, int> p{4, 5};
    assert(std::apply(add, p) == 9);
    auto t = std::make_tuple(1, 2);
    auto r = std::apply([](auto... xs) { return (xs + ...); }, t);
    assert(r == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_apply", run>;

}  // namespace
