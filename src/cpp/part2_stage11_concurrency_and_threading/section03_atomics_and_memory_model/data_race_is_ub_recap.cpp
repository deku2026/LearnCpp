// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : data_race_is_ub_recap
// Topic id : part2/stage11/section03/data_race_is_ub_recap
//
// Covers: data race is UB — safe fixes only

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::mutex m;
    int x = 0;
    std::thread t1([&] {
        for (int i = 0; i < 100; ++i) {
            std::lock_guard<std::mutex> g(m);
            ++x;
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 100; ++i) {
            std::lock_guard<std::mutex> g(m);
            ++x;
        }
    });
    t1.join();
    t2.join();
    assert(x == 200);
}

void demo_intermediate() {
    std::atomic<int> x{0};
    std::thread t1([&] {
        for (int i = 0; i < 100; ++i) {
            x.fetch_add(1);
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 100; ++i) {
            x.fetch_add(1);
        }
    });
    t1.join();
    t2.join();
    assert(x.load() == 200);
}

void demo_expert() {
    std::atomic<bool> done{false};
    int payload = 0;
    std::thread writer([&] {
        payload = 3;
        done.store(true, std::memory_order_release);
    });
    while (!done.load(std::memory_order_acquire)) {
    }
    assert(payload == 3);
    writer.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/data_race_is_ub_recap", run>;

}  // namespace
