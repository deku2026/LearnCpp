// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section03_atomic_helpers
// Item     : atomic_flag
// Topic id : part6/f/section03/atomic_flag
//
// Covers: std::atomic_flag as spinlock building block

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic_flag f = ATOMIC_FLAG_INIT;
    LEARN_CHECK(!f.test_and_set());
    LEARN_CHECK(f.test_and_set());
    f.clear();
    LEARN_CHECK(!f.test_and_set());
    f.clear();
}

void demo_intermediate() {
    std::atomic_flag lock = ATOMIC_FLAG_INIT;
    int counter = 0;
    auto critical = [&] {
        while (lock.test_and_set(std::memory_order_acquire)) {
        }
        ++counter;
        lock.clear(std::memory_order_release);
    };
    std::thread t1(critical), t2(critical);
    t1.join();
    t2.join();
    LEARN_CHECK(counter == 2);
}

void demo_expert() {
    std::atomic_flag f = ATOMIC_FLAG_INIT;
    f.clear(std::memory_order_release);
    LEARN_CHECK(!f.test_and_set(std::memory_order_acquire));
    f.clear();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section03/atomic_flag", run>;

}  // namespace
