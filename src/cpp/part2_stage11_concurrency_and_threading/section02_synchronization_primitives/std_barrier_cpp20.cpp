// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_barrier_cpp20
// Topic id : part2/stage11/section02/std_barrier_cpp20
//
// Covers: std::barrier reusable phases (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <barrier>
#include <cassert>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    std::barrier bar{2};
    std::atomic<int> phase{0};
    std::thread t([&] {
        bar.arrive_and_wait();
        phase.fetch_add(1);
    });
    bar.arrive_and_wait();
    phase.fetch_add(1);
    t.join();
    assert(phase.load() == 2);
#else
    std::atomic<int> phase{0};
    std::thread t([&] { phase.fetch_add(1); });
    t.join();
    phase.fetch_add(1);
    assert(phase.load() == 2);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    constexpr int n = 3;
    std::barrier bar{n};
    std::atomic<int> hits{0};
    std::vector<std::thread> ts;
    for (int i = 0; i < n; ++i) {
        ts.emplace_back([&] {
            bar.arrive_and_wait();
            hits.fetch_add(1);
            bar.arrive_and_wait();
            hits.fetch_add(10);
        });
    }
    for (auto& t : ts) {
        t.join();
    }
    assert(hits.load() == 33);
#else
    std::atomic<int> hits{0};
    std::vector<std::thread> ts;
    for (int i = 0; i < 3; ++i) {
        ts.emplace_back([&] {
            hits.fetch_add(1);
            hits.fetch_add(10);
        });
    }
    for (auto& t : ts) {
        t.join();
    }
    assert(hits.load() == 33);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    std::atomic<int> completions{0};
    std::barrier bar{2, [&]() noexcept { completions.fetch_add(1); }};
    std::thread t([&] { bar.arrive_and_wait(); });
    bar.arrive_and_wait();
    t.join();
    assert(completions.load() == 1);
#else
    std::atomic<int> completions{0};
    completions.fetch_add(1);
    assert(completions.load() == 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_barrier_cpp20", run>;

}  // namespace
