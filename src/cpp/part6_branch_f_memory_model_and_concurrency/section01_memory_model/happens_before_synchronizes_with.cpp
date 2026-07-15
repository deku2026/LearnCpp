// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : happens_before_synchronizes_with
// Topic id : part6/f/section01/happens_before_synchronizes_with
//
// Covers: synchronizes-with and happens-before relationships

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
    // release store synchronizes-with acquire load of the same atomic value.
    std::atomic<bool> done{false};
    int x = 0;
    std::thread writer([&] {
        x = 10;
        done.store(true, std::memory_order_release);
    });
    std::thread reader([&] {
        while (!done.load(std::memory_order_acquire)) {
        }
        assert(x == 10);  // happens-before guaranteed
    });
    writer.join();
    reader.join();
}

void demo_intermediate() {
    std::atomic<int> phase{0};
    int a = 0, b = 0;
    std::thread t1([&] {
        a = 1;
        phase.store(1, std::memory_order_release);
    });
    std::thread t2([&] {
        while (phase.load(std::memory_order_acquire) < 1) {
        }
        b = a;
    });
    t1.join();
    t2.join();
    assert(b == 1);
}

void demo_expert() {
    // join() also establishes happens-before with the completed thread.
    int x = 0;
    std::thread t([&] { x = 3; });
    t.join();
    assert(x == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/happens_before_synchronizes_with", run>;

}  // namespace
