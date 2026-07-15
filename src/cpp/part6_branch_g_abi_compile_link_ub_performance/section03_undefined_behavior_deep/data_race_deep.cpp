// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : data_race_deep
// Topic id : part6/g/section03/data_race_deep
//
// Covers: data races are UB; synchronize with mutex/atomic

#include "learn/topic_registry.hpp"

#include <atomic>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> c{0};
    auto inc = [&] {
        for (int i = 0; i < 1000; ++i) c.fetch_add(1, std::memory_order_relaxed);
    };
    std::thread t1(inc), t2(inc);
    t1.join();
    t2.join();
    LEARN_CHECK(c.load() == 2000);
}

void demo_intermediate() {
    int x = 0;
    std::mutex m;
    auto add = [&] {
        for (int i = 0; i < 500; ++i) {
            std::lock_guard<std::mutex> lock(m);
            ++x;
        }
    };
    std::thread t1(add), t2(add);
    t1.join();
    t2.join();
    LEARN_CHECK(x == 1000);
}

void demo_expert() {
    // Not executed: concurrent non-atomic writes without sync.
    std::atomic<bool> ready{false};
    int payload = 0;
    std::thread p([&] {
        payload = 11;
        ready.store(true, std::memory_order_release);
    });
    std::thread c([&] {
        while (!ready.load(std::memory_order_acquire)) {
        }
        LEARN_CHECK(payload == 11);
    });
    p.join();
    c.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/data_race_deep", run>;

}  // namespace
