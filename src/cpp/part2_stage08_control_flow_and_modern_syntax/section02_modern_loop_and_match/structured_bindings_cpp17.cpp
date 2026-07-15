// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : structured_bindings_cpp17
// Topic id : part2/stage08/section02/structured_bindings_cpp17
//
// Covers: structured bindings for pair/tuple/array/struct

#include "learn/topic_registry.hpp"

#include <array>
#include <map>
#include <string>
#include <tuple>
#include <utility>

namespace {

struct Point {
    int x;
    int y;
};

void demo_basics() {
    std::pair p{1, 2};
    auto [a, b] = p;
    LEARN_CHECK(a == 1);
    LEARN_CHECK(b == 2);

    auto t = std::make_tuple(3, 4.5, 'z');
    auto [i, d, c] = t;
    LEARN_CHECK(i == 3);
    LEARN_CHECK(d == 4.5);
    LEARN_CHECK(c == 'z');
}

void demo_intermediate() {
    Point pt{10, 20};
    auto [x, y] = pt;
    LEARN_CHECK(x == 10);
    LEARN_CHECK(y == 20);

    std::array<int, 3> arr{7, 8, 9};
    auto [u, v, w] = arr;
    LEARN_CHECK(u + v + w == 24);
}

void demo_expert() {
    std::map<std::string, int> m{{"one", 1}, {"two", 2}};
    int sum = 0;
    for (const auto& [k, val] : m) {
        (void)k;
        sum += val;
    }
    LEARN_CHECK(sum == 3);

    // Reference binding mutates original.
    Point q{1, 2};
    auto& [rx, ry] = q;
    rx = 5;
    LEARN_CHECK(q.x == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/structured_bindings_cpp17", run>;

}  // namespace
