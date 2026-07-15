// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : function_local_static_thread_safety
// Topic id : part6/f/section05/function_local_static_thread_safety
//
// Covers: C++11 thread-safe function-local static init

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

int& counter() {
    static int n = 0;
    return n;
}

void demo_basics() {
    counter() = 0;
    ++counter();
    LEARN_CHECK(counter() == 1);
}

void demo_intermediate() {
    static std::atomic<int> hits{0};
    auto& ref = []() -> int& {
        static int value = (hits.fetch_add(1), 0);
        return value;
    }();
    (void)ref;
    LEARN_CHECK(hits.load() >= 1);
}

void demo_expert() {
    std::atomic<int> seen{0};
    auto worker = [&] {
        static std::atomic<int> init_once{0};
        static int data = (init_once.fetch_add(1), 123);
        LEARN_CHECK(data == 123);
        seen.fetch_add(1);
    };
    std::thread t1(worker), t2(worker);
    t1.join();
    t2.join();
    LEARN_CHECK(seen.load() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/function_local_static_thread_safety", run>;

}  // namespace
