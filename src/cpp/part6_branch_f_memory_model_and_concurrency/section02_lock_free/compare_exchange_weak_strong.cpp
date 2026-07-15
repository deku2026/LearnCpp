// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : compare_exchange_weak_strong
// Topic id : part6/f/section02/compare_exchange_weak_strong
//
// Covers: compare_exchange_weak vs strong

#include "learn/topic_registry.hpp"

#include <atomic>

namespace {

void demo_basics() {
    std::atomic<int> x{1};
    int expected = 1;
    LEARN_CHECK(x.compare_exchange_strong(expected, 2));
    LEARN_CHECK(x.load() == 2);
}

void demo_intermediate() {
    std::atomic<int> x{5};
    int expected = 0;
    LEARN_CHECK(!x.compare_exchange_strong(expected, 1));
    LEARN_CHECK(expected == 5);  // updated with actual
    LEARN_CHECK(x.load() == 5);
}

void demo_expert() {
    std::atomic<int> x{0};
    int expected = 0;
    // weak may spuriously fail; loop until success
    while (!x.compare_exchange_weak(expected, 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
        expected = 0;
    }
    LEARN_CHECK(x.load() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/compare_exchange_weak_strong", run>;

}  // namespace
