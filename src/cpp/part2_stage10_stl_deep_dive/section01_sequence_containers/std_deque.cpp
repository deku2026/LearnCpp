// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_deque
// Topic id : part2/stage10/section01_sequence_containers/std_deque
//
// Covers: std::deque double-ended queue, O(1) push front/back

#include "learn/topic_registry.hpp"

#include <deque>

namespace {

void demo_basics() {
    std::deque<int> d;
    d.push_back(2);
    d.push_front(1);
    LEARN_CHECK(d.front() == 1);
    LEARN_CHECK(d.back() == 2);
    LEARN_CHECK(d.size() == 2);
}

void demo_intermediate() {
    std::deque<int> d{1, 2, 3};
    d.insert(d.begin() + 1, 9);
    LEARN_CHECK(d[1] == 9);
    d.pop_front();
    d.pop_back();
    LEARN_CHECK(d.front() == 9);
    LEARN_CHECK(d.back() == 2);
}

void demo_expert() {
    std::deque<int> d;
    for (int i = 0; i < 100; ++i) {
        d.push_front(i);
        d.push_back(-i);
    }
    LEARN_CHECK(d.size() == 200);
    LEARN_CHECK(d.front() == 99);
    LEARN_CHECK(d.back() == -99);
    // random access is supported
    LEARN_CHECK(d[0] == 99);
}

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
