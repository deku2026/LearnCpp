// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : std_atomic
// Topic id : part2/stage11/section03/std_atomic
//
// Covers: std::atomic load/store/fetch_add

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.store(5);
    assert(x.load() == 5);
    assert(x.fetch_add(3) == 5);
    assert(x.load() == 8);
}

void demo_intermediate() {
    std::atomic<int> counter{0};
    auto bump = [&] {
        for (int i = 0; i < 1000; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread t1(bump);
    std::thread t2(bump);
    t1.join();
    t2.join();
    assert(counter.load() == 2000);
}

void demo_expert() {
    std::atomic<int> flag{0};
    int payload = 0;
    std::thread producer([&] {
        payload = 42;
        flag.store(1, std::memory_order_release);
    });
    std::thread consumer([&] {
        while (flag.load(std::memory_order_acquire) == 0) {
        }
        assert(payload == 42);
    });
    producer.join();
    consumer.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/std_atomic", run>;

}  // namespace
