// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_queue
// Topic id : part2/stage10/section03_container_adapters/std_queue
//
// Covers: std::queue FIFO adapter, default deque

#include "learn/topic_registry.hpp"

#include <queue>

namespace {

void demo_basics() {
    std::queue<int> q;
    q.push(1);
    q.push(2);
    LEARN_CHECK(q.front() == 1);
    LEARN_CHECK(q.back() == 2);
    q.pop();
    LEARN_CHECK(q.front() == 2);
}

void demo_intermediate() {
    std::queue<int> q;
    for (int i = 0; i < 5; ++i) {
        q.push(i);
    }
    LEARN_CHECK(q.size() == 5);
    int sum = 0;
    while (!q.empty()) {
        sum += q.front();
        q.pop();
    }
    LEARN_CHECK(sum == 10);
}

void demo_expert() {
    std::queue<int> a;
    std::queue<int> b;
    a.push(1);
    b.push(2);
    b.push(3);
    a.swap(b);
    LEARN_CHECK(a.size() == 2);
    LEARN_CHECK(a.front() == 2);
    LEARN_CHECK(b.front() == 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/std_queue", run>;

}  // namespace
