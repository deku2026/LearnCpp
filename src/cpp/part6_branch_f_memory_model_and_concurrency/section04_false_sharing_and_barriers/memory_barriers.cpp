// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : memory_barriers
// Topic id : part6/f/section04/memory_barriers
//
// Covers: atomic thread fence as barrier

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.store(1, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    LEARN_CHECK(x.load(std::memory_order_relaxed) == 1);
}

void demo_intermediate() {
    std::atomic<bool> ready{false};
    int data = 0;
    std::thread prod([&] {
        data = 12;
        std::atomic_thread_fence(std::memory_order_release);
        ready.store(true, std::memory_order_relaxed);
    });
    std::thread cons([&] {
        while (!ready.load(std::memory_order_relaxed)) {
        }
        std::atomic_thread_fence(std::memory_order_acquire);
        LEARN_CHECK(data == 12);
    });
    prod.join();
    cons.join();
}

void demo_expert() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/memory_barriers", run>;

}  // namespace
