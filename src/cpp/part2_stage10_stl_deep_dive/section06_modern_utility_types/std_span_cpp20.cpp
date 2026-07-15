// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_span_cpp20
// Topic id : part2/stage10/section06_modern_utility_types/std_span_cpp20
//
// Covers: std::span contiguous non-owning view C++20

#include "learn/topic_registry.hpp"

#include <array>
#include <span>
#include <vector>

namespace {

void scale(std::span<int> s, int factor) {
    for (int& x : s) {
        x *= factor;
    }
}

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    scale(v, 2);
    LEARN_CHECK((v == std::vector<int>{2, 4, 6, 8}));
    int carr[] = {1, 2};
    scale(carr, 3);
    LEARN_CHECK(carr[0] == 3 && carr[1] == 6);
}

void demo_intermediate() {
    std::array<int, 5> a{1, 2, 3, 4, 5};
    std::span<int> sp = a;
    auto sub = sp.subspan(1, 3);
    LEARN_CHECK(sub.size() == 3);
    LEARN_CHECK(sub[0] == 2);
    LEARN_CHECK(sp.first(2).size() == 2);
    LEARN_CHECK(sp.last(2)[0] == 4);
}

void demo_expert() {
    std::vector<int> v{10, 20, 30};
    std::span<const int> csp = v;
    LEARN_CHECK(csp[1] == 20);
    std::span<int, 3> static_sp{v.data(), 3};
    LEARN_CHECK(static_sp.extent == 3);
    LEARN_CHECK(static_sp[2] == 30);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_span_cpp20", run>;

}  // namespace
