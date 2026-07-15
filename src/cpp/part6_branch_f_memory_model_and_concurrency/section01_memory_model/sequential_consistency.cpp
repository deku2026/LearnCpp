// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : sequential_consistency
// Topic id : part6/f/section01/sequential_consistency
//
// Covers: seq_cst default atomic ordering intuition

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.store(1);
    LEARN_CHECK(x.load() == 1);
}

void demo_intermediate() {
    std::atomic<int> counter{0};
    auto inc = [&] {
        for (int i = 0; i < 1000; ++i) counter.fetch_add(1);
    };
    std::thread t1(inc), t2(inc);
    t1.join();
    t2.join();
    LEARN_CHECK(counter.load() == 2000);
}

void demo_expert() {
    // seq_cst provides a single total order of seq_cst ops — strongest, sometimes costlier.
    std::atomic<int> a{0};
    std::atomic<int> b{0};
    std::thread t1([&] {
        a.store(1, std::memory_order_seq_cst);
        b.store(1, std::memory_order_seq_cst);
    });
    std::thread t2([&] {
        while (b.load(std::memory_order_seq_cst) == 0) {
        }
        LEARN_CHECK(a.load(std::memory_order_seq_cst) == 1);
    });
    t1.join();
    t2.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/sequential_consistency", run>;

}  // namespace
