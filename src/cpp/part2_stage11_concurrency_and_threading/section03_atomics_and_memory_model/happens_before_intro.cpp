// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : happens_before_intro
// Topic id : part2/stage11/section03/happens_before_intro
//
// Covers: happens-before via join/mutex

#include "learn/topic_registry.hpp"

#include <cassert>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    int x = 0;
    std::thread t([&] { x = 1; });
    t.join();
    assert(x == 1);
}

void demo_intermediate() {
    std::mutex m;
    int x = 0;
    std::thread t([&] {
        std::lock_guard<std::mutex> g(m);
        x = 2;
    });
    t.join();
    {
        std::lock_guard<std::mutex> g(m);
        assert(x == 2);
    }
}

void demo_expert() {
    std::mutex m;
    int seq = 0;
    std::thread t1([&] {
        std::lock_guard<std::mutex> g(m);
        seq = 1;
    });
    t1.join();
    std::thread t2([&] {
        std::lock_guard<std::mutex> g(m);
        assert(seq == 1);
        seq = 2;
    });
    t2.join();
    assert(seq == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/happens_before_intro", run>;

}  // namespace
