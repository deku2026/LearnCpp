// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_tuple_and_pair
// Topic id : part2/stage10/section06_modern_utility_types/std_tuple_and_pair
//
// Covers: std::tuple and std::pair structured binding

#include "learn/topic_registry.hpp"

#include <string>
#include <tuple>
#include <utility>

namespace {

void demo_basics() {
    std::pair<int, std::string> p{1, "a"};
    LEARN_CHECK(p.first == 1);
    LEARN_CHECK(p.second == "a");
    auto [x, y] = p;
    LEARN_CHECK(x == 1 && y == "a");
}

void demo_intermediate() {
    std::tuple<int, double, char> t{1, 2.5, 'z'};
    LEARN_CHECK(std::get<0>(t) == 1);
    LEARN_CHECK(std::get<double>(t) == 2.5);
    auto [a, b, c] = t;
    LEARN_CHECK(a == 1 && b == 2.5 && c == 'z');
}

void demo_expert() {
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::tuple_cat(t1, std::make_tuple(3));
    LEARN_CHECK(std::get<2>(t2) == 3);
    int x = 0;
    std::string s;
    std::tie(x, s) = std::make_pair(9, std::string{"ok"});
    LEARN_CHECK(x == 9 && s == "ok");
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
