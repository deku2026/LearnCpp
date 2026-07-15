// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_recursive_mutex
// Topic id : part2/stage11/section02/std_recursive_mutex
//
// Covers: std::recursive_mutex reentrant lock

#include "learn/topic_registry.hpp"

#include <cassert>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::recursive_mutex m;
    m.lock();
    m.lock();
    m.unlock();
    m.unlock();
}

void demo_intermediate() {
    std::recursive_mutex m;
    int depth = 0;
    auto rec = [&](auto&& self, int n) -> void {
        std::lock_guard<std::recursive_mutex> g(m);
        ++depth;
        if (n > 0) {
            self(self, n - 1);
        }
    };
    rec(rec, 3);
    assert(depth == 4);
}

void demo_expert() {
    std::recursive_mutex m;
    int v = 0;
    auto outer = [&] {
        std::lock_guard<std::recursive_mutex> g(m);
        auto inner = [&] {
            std::lock_guard<std::recursive_mutex> g2(m);
            ++v;
        };
        inner();
        ++v;
    };
    std::thread t1(outer);
    std::thread t2(outer);
    t1.join();
    t2.join();
    assert(v == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_recursive_mutex", run>;

}  // namespace
