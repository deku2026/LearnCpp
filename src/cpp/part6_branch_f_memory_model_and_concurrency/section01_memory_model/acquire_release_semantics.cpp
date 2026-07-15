// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : acquire_release_semantics
// Topic id : part6/f/section01/acquire_release_semantics
//
// Covers: release-acquire synchronizes-with for publishing data

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<bool> ready{false};
    int payload = 0;
    std::thread t1([&] {
        payload = 7;
        ready.store(true, std::memory_order_release);
    });
    std::thread t2([&] {
        while (!ready.load(std::memory_order_acquire)) {
        }
        assert(payload == 7);
    });
    t1.join();
    t2.join();
}

void demo_intermediate() {
    std::atomic<int> handoff{0};
    int value = 0;
    std::thread prod([&] {
        value = 99;
        handoff.store(1, std::memory_order_release);
    });
    std::thread cons([&] {
        while (handoff.load(std::memory_order_acquire) == 0) {
        }
        assert(value == 99);
    });
    prod.join();
    cons.join();
}

void demo_expert() {
    // Pairing: store release with load acquire on the same atomic.
    std::atomic<int> gate{0};
    int x = 0;
    std::thread a([&] {
        x = 1;
        gate.fetch_add(1, std::memory_order_release);
    });
    std::thread b([&] {
        while (gate.load(std::memory_order_acquire) == 0) {
        }
        assert(x == 1);
    });
    a.join();
    b.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/acquire_release_semantics", run>;

}  // namespace
