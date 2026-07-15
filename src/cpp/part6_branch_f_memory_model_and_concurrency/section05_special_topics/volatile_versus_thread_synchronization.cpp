// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : volatile_versus_thread_synchronization
// Topic id : part6/f/section05/volatile_versus_thread_synchronization
//
// Covers: volatile is not for inter-thread sync; use atomics

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    // volatile: suppress certain optimizations for MMIO/setjmp — not a mutex.
    volatile int device = 0;
    device = 1;
    LEARN_CHECK(device == 1);
}

void demo_intermediate() {
    std::atomic<int> ready{0};
    int data = 0;
    std::thread t1([&] {
        data = 4;
        ready.store(1);
    });
    std::thread t2([&] {
        while (ready.load() == 0) {
        }
        LEARN_CHECK(data == 4);
    });
    t1.join();
    t2.join();
}

void demo_expert() {
    // Teaching: volatile int shared; // does NOT prevent data races.
    std::atomic<int> shared{0};
    shared.store(1);
    LEARN_CHECK(shared.load() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/volatile_versus_thread_synchronization", run>;

}  // namespace
