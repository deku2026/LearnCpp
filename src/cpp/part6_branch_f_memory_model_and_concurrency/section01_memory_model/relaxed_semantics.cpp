// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : relaxed_semantics
// Topic id : part6/f/section01/relaxed_semantics
//
// Covers: memory_order_relaxed for pure counters without publishing

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> c{0};
    c.fetch_add(1, std::memory_order_relaxed);
    LEARN_CHECK(c.load(std::memory_order_relaxed) == 1);
}

void demo_intermediate() {
    std::atomic<int> c{0};
    auto work = [&] {
        for (int i = 0; i < 1000; ++i) c.fetch_add(1, std::memory_order_relaxed);
    };
    std::thread t1(work), t2(work);
    t1.join();
    t2.join();
    LEARN_CHECK(c.load() == 2000);
}

void demo_expert() {
    // relaxed does not synchronize-with; do not use alone to publish non-atomic data.
    std::atomic<int> c{0};
    c.store(3, std::memory_order_relaxed);
    LEARN_CHECK(c.load(std::memory_order_relaxed) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/relaxed_semantics", run>;

}  // namespace
