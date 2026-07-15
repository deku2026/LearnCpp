// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : std_call_once
// Topic id : part6/f/section05/std_call_once
//
// Covers: std::call_once / once_flag

#include "learn/topic_registry.hpp"

#include <cassert>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::once_flag flag;
    int n = 0;
    std::call_once(flag, [&] { n = 1; });
    std::call_once(flag, [&] { n = 2; });
    assert(n == 1);
}

void demo_intermediate() {
    std::once_flag flag;
    int n = 0;
    auto job = [&] { std::call_once(flag, [&] { ++n; }); };
    std::thread t1(job), t2(job), t3(job);
    t1.join();
    t2.join();
    t3.join();
    assert(n == 1);
}

void demo_expert() {
    std::once_flag flag;
    static int* p = nullptr;
    std::call_once(flag, [] { p = new int(9); });
    assert(p && *p == 9);
    delete p;
    p = nullptr;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/std_call_once", run>;

}  // namespace
