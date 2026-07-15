// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : cas_loop
// Topic id : part6/f/section02/cas_loop
//
// Covers: CAS loop for lock-free update

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void atomic_max(std::atomic<int>& a, int v) {
    int cur = a.load(std::memory_order_relaxed);
    while (cur < v && !a.compare_exchange_weak(cur, v, std::memory_order_relaxed)) {
    }
}

void demo_basics() {
    std::atomic<int> a{3};
    atomic_max(a, 5);
    assert(a.load() == 5);
    atomic_max(a, 4);
    assert(a.load() == 5);
}

void demo_intermediate() {
    std::atomic<int> a{0};
    auto worker = [&](int v) { atomic_max(a, v); };
    std::thread t1(worker, 10), t2(worker, 20), t3(worker, 15);
    t1.join();
    t2.join();
    t3.join();
    assert(a.load() == 20);
}

void demo_expert() {
    std::atomic<int> sum{0};
    auto add = [&](int n) {
        int cur = sum.load();
        while (!sum.compare_exchange_weak(cur, cur + n)) {
        }
    };
    std::thread t1(add, 5), t2(add, 7);
    t1.join();
    t2.join();
    assert(sum.load() == 12);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/cas_loop", run>;

}  // namespace
