// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_queue
// Topic id : part2/stage10/section03_container_adapters/std_queue
//
// Covers: std::queue FIFO adapter, default deque

#include "learn/topic_registry.hpp"

#include <cassert>
#include <queue>

namespace {

void demo_basics() {
    std::queue<int> q;
    q.push(1);
    q.push(2);
    assert(q.front() == 1);
    assert(q.back() == 2);
    q.pop();
    assert(q.front() == 2);
}

void demo_intermediate() {
    std::queue<int> q;
    for (int i = 0; i < 5; ++i) {
        q.push(i);
    }
    assert(q.size() == 5);
    int sum = 0;
    while (!q.empty()) {
        sum += q.front();
        q.pop();
    }
    assert(sum == 10);
}

void demo_expert() {
    std::queue<int> a;
    std::queue<int> b;
    a.push(1);
    b.push(2);
    b.push(3);
    a.swap(b);
    assert(a.size() == 2);
    assert(a.front() == 2);
    assert(b.front() == 1);
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
