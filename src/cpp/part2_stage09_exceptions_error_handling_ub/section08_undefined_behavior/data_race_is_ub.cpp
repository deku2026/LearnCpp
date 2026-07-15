// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : data_race_is_ub
// Topic id : part2/stage09/section08/data_race_is_ub
//
// Covers: data races are UB — use mutex/atomic, not racy shared writes

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
    std::atomic<int> counter{0};
    auto inc = [&] {
        for (int i = 0; i < 1000; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread t1(inc);
    std::thread t2(inc);
    t1.join();
    t2.join();
    assert(counter.load() == 2000);
}

void demo_intermediate() {
    int protected_value = 0;
    std::mutex m;
    auto add = [&](int n) {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> lock(m);
            ++protected_value;
        }
    };
    std::thread t1(add, 500);
    std::thread t2(add, 500);
    t1.join();
    t2.join();
    assert(protected_value == 1000);
}

void demo_expert() {
    // Comment only: two threads writing non-atomic int without sync = data race = UB.
    // We only demonstrate synchronized patterns above.
    std::atomic<bool> ready{false};
    int payload = 0;
    std::thread producer([&] {
        payload = 42;  // happens-before via release/acquire
        ready.store(true, std::memory_order_release);
    });
    std::thread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/data_race_is_ub", run>;

}  // namespace
