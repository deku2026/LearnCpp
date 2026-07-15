// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_tuple_and_pair
// Topic id : part2/stage10/section06_modern_utility_types/std_tuple_and_pair
//
// Covers: std::tuple and std::pair structured binding

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <tuple>
#include <utility>

namespace {

void demo_basics() {
    std::pair<int, std::string> p{1, "a"};
    assert(p.first == 1);
    assert(p.second == "a");
    auto [x, y] = p;
    assert(x == 1 && y == "a");
}

void demo_intermediate() {
    std::tuple<int, double, char> t{1, 2.5, 'z'};
    assert(std::get<0>(t) == 1);
    assert(std::get<double>(t) == 2.5);
    auto [a, b, c] = t;
    assert(a == 1 && b == 2.5 && c == 'z');
}

void demo_expert() {
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::tuple_cat(t1, std::make_tuple(3));
    assert(std::get<2>(t2) == 3);
    int x = 0;
    std::string s;
    std::tie(x, s) = std::make_pair(9, std::string{"ok"});
    assert(x == 9 && s == "ok");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_tuple_and_pair", run>;

}  // namespace
