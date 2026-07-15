// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_relaxed
// Topic id : part2/stage11/section03/memory_order_relaxed
//
// Covers: memory_order_relaxed counters

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
    auto bump = [&] {
        for (int i = 0; i < 1000; ++i) {
            c.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread t1(bump);
    std::thread t2(bump);
    t1.join();
    t2.join();
    LEARN_CHECK(c.load(std::memory_order_relaxed) == 2000);
}

void demo_expert() {
    std::atomic<long long> hits{0};
    std::thread t([&] {
        for (int i = 0; i < 100; ++i) {
            hits.fetch_add(1, std::memory_order_relaxed);
        }
    });
    t.join();
    LEARN_CHECK(hits.load(std::memory_order_relaxed) == 100);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_relaxed", run>;

}  // namespace
