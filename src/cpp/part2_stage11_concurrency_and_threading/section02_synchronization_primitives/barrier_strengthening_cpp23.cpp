// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : barrier_strengthening_cpp23
// Topic id : part2/stage11/section02/barrier_strengthening_cpp23
//
// Covers: barrier completion function notes

#include "learn/topic_registry.hpp"

#include <atomic>
#include <barrier>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    std::atomic<int> c{0};
    std::barrier bar{1, [&]() noexcept { c.fetch_add(1); }};
    bar.arrive_and_wait();
    LEARN_CHECK(c.load() == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    std::atomic<int> phases{0};
    std::barrier bar{2, [&]() noexcept { phases.fetch_add(1); }};
    std::thread t([&] {
        bar.arrive_and_wait();
        bar.arrive_and_wait();
    });
    bar.arrive_and_wait();
    bar.arrive_and_wait();
    t.join();
    LEARN_CHECK(phases.load() == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    std::atomic<int> token{0};
    std::barrier bar{2, [&]() noexcept { token.store(7); }};
    std::thread t([&] { bar.arrive_and_wait(); });
    bar.arrive_and_wait();
    t.join();
    LEARN_CHECK(token.load() == 7);
#else
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/barrier_strengthening_cpp23", run>;

}  // namespace
