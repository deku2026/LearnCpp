// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : std_atomic
// Topic id : part2/stage11/section03/std_atomic
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/std_atomic";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::atomic<int> counter{};
    constexpr int workers = 4;
    constexpr int increments = 1'000;
    std::array<std::jthread, workers> threads;
    for (auto& thread : threads) {
        thread = std::jthread{[&counter] {
            for (int index = 0; index < increments; ++index) {
                ++counter;  // atomic read-modify-write; default is seq_cst
            }
        }};
    }
    for (auto& thread : threads) {
        thread.join();
    }

    LEARN_EXPECT_EQ(checks, counter.load(), workers * increments);
    counter.store(7);
    LEARN_EXPECT_EQ(checks, counter.exchange(9), 7);
    LEARN_EXPECT_EQ(checks, counter.load(), 9);

    // is_lock_free is an implementation property; correctness must never rely
    // on it returning true.
    LEARN_EXPECT_EQ(checks, counter.is_lock_free(), std::atomic<int>::is_always_lock_free);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/std_atomic", run>;

}  // namespace
