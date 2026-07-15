// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_acq_rel
// Topic id : part2/stage11/section03/memory_order_acq_rel
//
// Covers: memory_order_acq_rel RMW

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    int prev = x.fetch_add(1, std::memory_order_acq_rel);
    LEARN_CHECK(prev == 0);
    LEARN_CHECK(x.load() == 1);
}

void demo_intermediate() {
    std::atomic<int> x{0};
    std::thread t([&] { x.fetch_add(1, std::memory_order_acq_rel); });
    x.fetch_add(1, std::memory_order_acq_rel);
    t.join();
    LEARN_CHECK(x.load() == 2);
}

void demo_expert() {
    std::atomic<int> gate{0};
    int data = 0;
    std::thread t([&] {
        data = 5;
        gate.fetch_add(1, std::memory_order_acq_rel);
    });
    while (gate.load(std::memory_order_acquire) == 0) {
    }
    LEARN_CHECK(data == 5);
    t.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_acq_rel", run>;

}  // namespace
