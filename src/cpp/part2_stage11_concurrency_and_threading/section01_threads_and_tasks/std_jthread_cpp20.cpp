// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_jthread_cpp20
// Topic id : part2/stage11/section01/std_jthread_cpp20
//
// Covers: std::jthread RAII auto-join (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<int> v{0};
    {
        std::jthread t([&] { v.store(1); });
    }
    assert(v.load() == 1);
#else
    std::atomic<int> v{0};
    std::thread t([&] { v.store(1); });
    t.join();
    assert(v.load() == 1);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<int> sum{0};
    {
        std::jthread t1([&] { sum.fetch_add(3); });
        std::jthread t2([&] { sum.fetch_add(4); });
    }
    assert(sum.load() == 7);
#else
    std::atomic<int> sum{0};
    std::thread t1([&] { sum.fetch_add(3); });
    std::thread t2([&] { sum.fetch_add(4); });
    t1.join();
    t2.join();
    assert(sum.load() == 7);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<bool> ran{false};
    {
        std::jthread t([&](std::stop_token) { ran.store(true); });
        assert(t.joinable());
    }
    assert(ran.load());
#else
    std::atomic<bool> ran{false};
    std::thread t([&] { ran.store(true); });
    t.join();
    assert(ran.load());
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_jthread_cpp20", run>;

}  // namespace
