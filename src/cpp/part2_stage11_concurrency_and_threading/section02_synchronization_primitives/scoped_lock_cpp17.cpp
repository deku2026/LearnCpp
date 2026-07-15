// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : scoped_lock_cpp17
// Topic id : part2/stage11/section02/scoped_lock_cpp17
//
// Covers: std::scoped_lock multi-mutex (C++17)

#include "learn/topic_registry.hpp"

#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::mutex m1;
    std::mutex m2;
    int a = 0;
    int b = 0;
    {
        std::scoped_lock g(m1, m2);
        a = 1;
        b = 2;
    }
    LEARN_CHECK(a == 1 && b == 2);
}

void demo_intermediate() {
    std::mutex m1;
    std::mutex m2;
    int x = 0;
    auto lr = [&] {
        for (int i = 0; i < 200; ++i) {
            std::scoped_lock g(m1, m2);
            ++x;
        }
    };
    auto rl = [&] {
        for (int i = 0; i < 200; ++i) {
            std::scoped_lock g(m2, m1);
            ++x;
        }
    };
    std::thread t1(lr);
    std::thread t2(rl);
    t1.join();
    t2.join();
    LEARN_CHECK(x == 400);
}

void demo_expert() {
    std::mutex m;
    {
        std::scoped_lock g(m);
    }
    std::scoped_lock empty_pack{};
    (void)empty_pack;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/scoped_lock_cpp17", run>;

}  // namespace
