// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_call_once
// Topic id : part2/stage11/section02/std_call_once
//
// Covers: std::call_once / once_flag

#include "learn/topic_registry.hpp"

#include <cassert>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace {

void demo_basics() {
    std::once_flag flag;
    int n = 0;
    auto init = [&] { ++n; };
    std::call_once(flag, init);
    std::call_once(flag, init);
    assert(n == 1);
}

void demo_intermediate() {
    std::once_flag flag;
    int n = 0;
    auto bump = [&] { ++n; };
    std::thread t1([&] { std::call_once(flag, bump); });
    std::thread t2([&] { std::call_once(flag, bump); });
    std::thread t3([&] { std::call_once(flag, bump); });
    t1.join();
    t2.join();
    t3.join();
    assert(n == 1);
}

void demo_expert() {
    std::once_flag flag;
    int attempts = 0;
    int success = 0;
    auto flaky = [&] {
        ++attempts;
        if (attempts == 1) {
            throw std::runtime_error("fail once");
        }
        ++success;
    };
    try {
        std::call_once(flag, flaky);
    } catch (...) {
    }
    std::call_once(flag, flaky);
    assert(success == 1);
    assert(attempts == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_call_once", run>;

}  // namespace
