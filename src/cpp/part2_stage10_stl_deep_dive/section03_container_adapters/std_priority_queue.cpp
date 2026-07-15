// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_priority_queue
// Topic id : part2/stage10/section03_container_adapters/std_priority_queue
//
// Covers: std::priority_queue heap adapter, max-heap by default

#include "learn/topic_registry.hpp"

#include <functional>
#include <queue>
#include <vector>

namespace {

void demo_basics() {
    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    LEARN_CHECK(pq.top() == 4);
    pq.pop();
    LEARN_CHECK(pq.top() == 3);
}

void demo_intermediate() {
    std::priority_queue<int, std::vector<int>, std::greater<int>> minq;
    minq.push(3);
    minq.push(1);
    minq.push(4);
    LEARN_CHECK(minq.top() == 1);
}

void demo_expert() {
    std::vector<int> data{5, 1, 9, 3};
    std::priority_queue<int> pq(std::less<int>(), data);
    LEARN_CHECK(pq.size() == 4);
    LEARN_CHECK(pq.top() == 9);
    pq.pop();
    LEARN_CHECK(pq.top() == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/std_priority_queue", run>;

}  // namespace
