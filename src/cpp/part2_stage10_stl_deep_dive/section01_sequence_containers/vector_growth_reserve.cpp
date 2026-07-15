// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_growth_reserve
// Topic id : part2/stage10/section01_sequence_containers/vector_growth_reserve
//
// Covers: vector capacity growth, reserve vs resize, reallocation cost

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v;
    LEARN_CHECK(v.size() == 0);
    LEARN_CHECK(v.capacity() == 0);
    v.push_back(1);
    LEARN_CHECK(v.size() == 1);
    LEARN_CHECK(v.capacity() >= 1);
    v.reserve(32);
    LEARN_CHECK(v.capacity() >= 32);
    LEARN_CHECK(v.size() == 1);
}

void demo_intermediate() {
    std::vector<int> v;
    v.reserve(100);
    const auto cap = v.capacity();
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }
    LEARN_CHECK(v.size() == 100);
    LEARN_CHECK(v.capacity() == cap);
    v.resize(50);
    LEARN_CHECK(v.size() == 50);
    LEARN_CHECK(v.capacity() >= 100);
    v.shrink_to_fit();
    LEARN_CHECK(v.size() == 50);
}

void demo_expert() {
    std::vector<int> v;
    std::size_t prev = 0;
    int growths = 0;
    for (int i = 0; i < 64; ++i) {
        v.push_back(i);
        if (v.capacity() != prev) {
            ++growths;
            prev = v.capacity();
        }
    }
    LEARN_CHECK(growths >= 1);
    LEARN_CHECK(v.capacity() >= v.size());
    // reserve only grows capacity, never shrinks
    const auto c = v.capacity();
    v.reserve(10);
    LEARN_CHECK(v.capacity() == c);
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
    ::learn::topic<"part2/stage10/section01_sequence_containers/vector_growth_reserve", run>;

}  // namespace
