// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : lock_guard
// Topic id : part2/stage11/section02/lock_guard
//
// Covers: std::lock_guard RAII mutex

#include "learn/topic_registry.hpp"

#include <cassert>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::mutex m;
    int x = 0;
    {
        std::lock_guard<std::mutex> g(m);
        x = 1;
    }
    assert(x == 1);
}

void demo_intermediate() {
    std::mutex m;
    int sum = 0;
    auto add = [&](int n) {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> g(m);
            ++sum;
        }
    };
    std::thread t1(add, 500);
    std::thread t2(add, 500);
    t1.join();
    t2.join();
    assert(sum == 1000);
}

void demo_expert() {
    std::mutex m;
    int y = 0;
    try {
        std::lock_guard<std::mutex> g(m);
        y = 2;
        throw 1;
    } catch (...) {
    }
    std::lock_guard<std::mutex> g(m);
    assert(y == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/lock_guard", run>;

}  // namespace
