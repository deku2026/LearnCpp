// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : seq_cst_semantics
// Topic id : part6/f/section01/seq_cst_semantics
//
// Covers: memory_order_seq_cst loads/stores/RMW

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> v{0};
    v.store(5, std::memory_order_seq_cst);
    LEARN_CHECK(v.load(std::memory_order_seq_cst) == 5);
}

void demo_intermediate() {
    std::atomic<int> v{1};
    int expected = 1;
    LEARN_CHECK(v.compare_exchange_strong(expected, 2, std::memory_order_seq_cst));
    LEARN_CHECK(v.load() == 2);
}

void demo_expert() {
    std::atomic<int> flag{0};
    int data = 0;
    std::thread prod([&] {
        data = 42;
        flag.store(1, std::memory_order_seq_cst);
    });
    std::thread cons([&] {
        while (flag.load(std::memory_order_seq_cst) == 0) {
        }
        LEARN_CHECK(data == 42);
    });
    prod.join();
    cons.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/seq_cst_semantics", run>;

}  // namespace
