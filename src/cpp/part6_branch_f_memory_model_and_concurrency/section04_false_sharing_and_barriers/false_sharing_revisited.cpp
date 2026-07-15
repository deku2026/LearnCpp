// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : false_sharing_revisited
// Topic id : part6/f/section04/false_sharing_revisited
//
// Covers: false sharing revisited with padded atomics

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

struct alignas(64) Slot {
    std::atomic<int> v{0};
};

void demo_basics() {
    Slot a, b;
    a.v = 1;
    b.v = 2;
    assert(a.v.load() + b.v.load() == 3);
}

void demo_intermediate() {
    Slot s1, s2;
    std::thread t1([&] {
        for (int i = 0; i < 2000; ++i) s1.v.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread t2([&] {
        for (int i = 0; i < 2000; ++i) s2.v.fetch_add(1, std::memory_order_relaxed);
    });
    t1.join();
    t2.join();
    assert(s1.v.load() == 2000 && s2.v.load() == 2000);
}

void demo_expert() {
    assert(sizeof(Slot) >= 64);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/false_sharing_revisited", run>;

}  // namespace
