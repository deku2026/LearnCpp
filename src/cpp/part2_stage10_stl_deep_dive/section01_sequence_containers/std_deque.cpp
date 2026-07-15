// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_deque
// Topic id : part2/stage10/section01_sequence_containers/std_deque
//
// Covers: std::deque double-ended queue, O(1) push front/back

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>

namespace {

void demo_basics() {
    std::deque<int> d;
    d.push_back(2);
    d.push_front(1);
    assert(d.front() == 1);
    assert(d.back() == 2);
    assert(d.size() == 2);
}

void demo_intermediate() {
    std::deque<int> d{1, 2, 3};
    d.insert(d.begin() + 1, 9);
    assert(d[1] == 9);
    d.pop_front();
    d.pop_back();
    assert(d.front() == 9);
    assert(d.back() == 2);
}

void demo_expert() {
    std::deque<int> d;
    for (int i = 0; i < 100; ++i) {
        d.push_front(i);
        d.push_back(-i);
    }
    assert(d.size() == 200);
    assert(d.front() == 99);
    assert(d.back() == -99);
    // random access is supported
    assert(d[0] == 99);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01_sequence_containers/std_deque", run>;

}  // namespace
