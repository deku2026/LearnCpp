// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : structured_bindings
// Topic id : part3/section03/structured_bindings
//
// Covers: structured bindings for pair/tuple/array/struct

#include "learn/topic_registry.hpp"

#include <array>
#include <map>
#include <string>
#include <utility>

namespace {

void demo_basics() {
    std::pair<int, int> p{1, 2};
    auto [x, y] = p;
    LEARN_CHECK(x == 1 && y == 2);
}

void demo_intermediate() {
    std::array<int, 3> a{10, 20, 30};
    auto [i, j, k] = a;
    LEARN_CHECK(i + j + k == 60);
    std::map<std::string, int> m{{"a", 1}};
    for (const auto& [key, val] : m) {
        LEARN_CHECK(key == "a" && val == 1);
    }
}

void demo_expert() {
    struct Point {
        int x;
        int y;
    };
    Point pt{3, 4};
    auto& [rx, ry] = pt;
    rx = 9;
    LEARN_CHECK(pt.x == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/structured_bindings", run>;

}  // namespace
