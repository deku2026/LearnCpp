// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_array
// Topic id : part2/stage10/section01_sequence_containers/std_array
//
// Covers: std::array fixed-size contiguous container, zero overhead vs C array

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <numeric>

namespace {

void demo_basics() {
    std::array<int, 4> a{1, 2, 3, 4};
    assert(a.size() == 4);
    assert(a[0] == 1);
    assert(a.front() == 1);
    assert(a.back() == 4);
    assert(a.data() != nullptr);
}

void demo_intermediate() {
    std::array<int, 3> a{};
    assert(a[0] == 0 && a[1] == 0 && a[2] == 0);
    a.fill(7);
    assert(a[0] == 7 && a[2] == 7);
    std::array<int, 3> b{1, 2, 3};
    a.swap(b);
    assert(a[0] == 1);
    assert(b[0] == 7);
}

void demo_expert() {
    constexpr std::array<int, 5> a{1, 2, 3, 4, 5};
    static_assert(a.size() == 5);
    static_assert(a[2] == 3);
    const int sum = std::accumulate(a.begin(), a.end(), 0);
    assert(sum == 15);
    assert(std::get<0>(a) == 1);
    assert(std::get<4>(a) == 5);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01_sequence_containers/std_array", run>;

}  // namespace
