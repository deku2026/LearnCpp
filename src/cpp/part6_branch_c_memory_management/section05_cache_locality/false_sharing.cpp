// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : false_sharing
// Topic id : part6/c/section05/false_sharing
//
// Covers: false sharing avoided by padding; measure via separated atomics

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

struct alignas(64) PaddedCounter {
    std::atomic<int> v{0};
};

void demo_basics() {
    PaddedCounter a;
    PaddedCounter b;
    a.v.store(1);
    b.v.store(2);
    LEARN_CHECK(a.v.load() + b.v.load() == 3);
}

void demo_intermediate() {
    PaddedCounter c1;
    PaddedCounter c2;
    std::thread t1([&] {
        for (int i = 0; i < 1000; ++i) c1.v.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; ++i) c2.v.fetch_add(1, std::memory_order_relaxed);
    });
    t1.join();
    t2.join();
    LEARN_CHECK(c1.v.load() == 1000);
    LEARN_CHECK(c2.v.load() == 1000);
}

void demo_expert() {
    // Adjacent atomics without padding may share a cache line (false sharing).
    // We only run the padded safe pattern above.
    LEARN_CHECK(sizeof(PaddedCounter) >= 64);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/false_sharing", run>;

}  // namespace
