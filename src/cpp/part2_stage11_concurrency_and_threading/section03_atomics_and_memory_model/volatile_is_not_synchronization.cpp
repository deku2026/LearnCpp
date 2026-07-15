// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : volatile_is_not_synchronization
// Topic id : part2/stage11/section03/volatile_is_not_synchronization
//
// Covers: volatile != synchronization

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    volatile int v = 1;
    LEARN_CHECK(v == 1);
    std::atomic<int> a{1};
    LEARN_CHECK(a.load() == 1);
}

void demo_intermediate() {
    std::atomic<int> flag{0};
    int data = 0;
    std::thread t([&] {
        data = 5;
        flag.store(1, std::memory_order_release);
    });
    while (flag.load(std::memory_order_acquire) == 0) {
    }
    LEARN_CHECK(data == 5);
    t.join();
}

void demo_expert() {
    std::atomic<bool> stop{false};
    std::atomic<int> ticks{0};
    std::thread worker([&] {
        while (!stop.load(std::memory_order_acquire)) {
            ticks.fetch_add(1, std::memory_order_relaxed);
        }
    });
    stop.store(true, std::memory_order_release);
    worker.join();
    LEARN_CHECK(ticks.load() >= 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/volatile_is_not_synchronization", run>;

}  // namespace
