// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : modification_order
// Topic id : part6/f/section01/modification_order
//
// Covers: modification order of a single atomic object

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.store(1);
    x.store(2);
    LEARN_CHECK(x.load() == 2);
}

void demo_intermediate() {
    std::atomic<int> x{0};
    std::thread t1([&] { x.store(1, std::memory_order_relaxed); });
    std::thread t2([&] { x.store(2, std::memory_order_relaxed); });
    t1.join();
    t2.join();
    int v = x.load();
    LEARN_CHECK(v == 1 || v == 2);
}

void demo_expert() {
    // All threads agree on a single modification order per atomic object.
    std::atomic<int> x{0};
    x.fetch_add(1);
    x.fetch_add(1);
    LEARN_CHECK(x.load() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/modification_order", run>;

}  // namespace
