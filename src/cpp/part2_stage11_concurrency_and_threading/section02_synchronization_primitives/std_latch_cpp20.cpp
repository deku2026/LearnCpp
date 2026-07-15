// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_latch_cpp20
// Topic id : part2/stage11/section02/std_latch_cpp20
//
// Covers: std::latch one-shot (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <latch>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
#if defined(__cpp_lib_latch) && __cpp_lib_latch >= 201907L
    std::latch done{1};
    std::atomic<int> v{0};
    std::thread t([&] {
        v.store(1);
        done.count_down();
    });
    done.wait();
    assert(v.load() == 1);
    t.join();
#else
    std::atomic<int> v{0};
    std::thread t([&] { v.store(1); });
    t.join();
    assert(v.load() == 1);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_latch) && __cpp_lib_latch >= 201907L
    constexpr int n = 4;
    std::latch done{n};
    std::atomic<int> sum{0};
    std::vector<std::thread> ts;
    for (int i = 0; i < n; ++i) {
        ts.emplace_back([&] {
            sum.fetch_add(1);
            done.count_down();
        });
    }
    done.wait();
    assert(sum.load() == n);
    for (auto& t : ts) {
        t.join();
    }
#else
    std::atomic<int> sum{0};
    std::vector<std::thread> ts;
    for (int i = 0; i < 4; ++i) {
        ts.emplace_back([&] { sum.fetch_add(1); });
    }
    for (auto& t : ts) {
        t.join();
    }
    assert(sum.load() == 4);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_latch) && __cpp_lib_latch >= 201907L
    std::latch start{1};
    std::atomic<int> ran{0};
    std::thread t([&] {
        start.wait();
        ran.fetch_add(1);
    });
    assert(ran.load() == 0);
    start.count_down();
    t.join();
    assert(ran.load() == 1);
#else
    std::atomic<int> ran{0};
    std::thread t([&] { ran.fetch_add(1); });
    t.join();
    assert(ran.load() == 1);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_latch_cpp20", run>;

}  // namespace
