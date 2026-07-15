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
#include <cassert>
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
    assert(a == 1);
    assert(b == 2);

    auto t = std::make_tuple(3, 4.5, 'z');
    auto [i, d, c] = t;
    assert(i == 3);
    assert(d == 4.5);
    assert(c == 'z');
}

void demo_intermediate() {
    Point pt{10, 20};
    auto [x, y] = pt;
    assert(x == 10);
    assert(y == 20);

    std::array<int, 3> arr{7, 8, 9};
    auto [u, v, w] = arr;
    assert(u + v + w == 24);
}

void demo_expert() {
    std::map<std::string, int> m{{"one", 1}, {"two", 2}};
    int sum = 0;
    for (const auto& [k, val] : m) {
        (void)k;
        sum += val;
    }
    assert(sum == 3);

    // Reference binding mutates original.
    Point q{1, 2};
    auto& [rx, ry] = q;
    rx = 5;
    assert(q.x == 5);
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
