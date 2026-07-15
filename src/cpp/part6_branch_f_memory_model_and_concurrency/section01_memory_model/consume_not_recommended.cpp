// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : consume_not_recommended
// Topic id : part6/f/section01/consume_not_recommended
//
// Covers: memory_order_consume discouraged; prefer acquire

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
    // consume is not recommended; compilers often upgrade to acquire.
    std::atomic<int> ready{0};
    int data = 0;
    std::thread t1([&] {
        data = 5;
        ready.store(1, std::memory_order_release);
    });
    std::thread t2([&] {
        while (ready.load(std::memory_order_acquire) == 0) {
        }
        assert(data == 5);
    });
    t1.join();
    t2.join();
}

void demo_intermediate() {
    // Document: consume intended for dependency-ordered reads; hard to use correctly.
    assert(true);
}

void demo_expert() {
    std::atomic<int> x{0};
    x.store(1, std::memory_order_release);
    assert(x.load(std::memory_order_acquire) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/consume_not_recommended", run>;

}  // namespace
