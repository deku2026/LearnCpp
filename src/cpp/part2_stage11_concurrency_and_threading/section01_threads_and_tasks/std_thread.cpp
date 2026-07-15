// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_thread
// Topic id : part2/stage11/section01/std_thread
//
// Covers: std::thread start/join, args, joinable

#include "learn/topic_registry.hpp"

#include <cassert>
#include <numeric>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
    int result = 0;
    std::thread t([&result] { result = 42; });
    t.join();
    assert(result == 42);
    assert(!t.joinable());
}

void demo_intermediate() {
    int a = 0;
    int b = 0;
    std::thread t1([&] { a = 1; });
    std::thread t2([&] { b = 2; });
    t1.join();
    t2.join();
    assert(a == 1 && b == 2);

    int sum = 0;
    std::thread t3([](int x, int y, int* out) { *out = x + y; }, 3, 4, &sum);
    t3.join();
    assert(sum == 7);
}

void demo_expert() {
    constexpr int n = 4;
    std::vector<int> partial(static_cast<std::size_t>(n), 0);
    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        workers.emplace_back([&partial, i] { partial[static_cast<std::size_t>(i)] = i + 1; });
    }
    for (auto& w : workers) {
        w.join();
    }
    const int total = std::accumulate(partial.begin(), partial.end(), 0);
    assert(total == 10);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_thread", run>;

}  // namespace
