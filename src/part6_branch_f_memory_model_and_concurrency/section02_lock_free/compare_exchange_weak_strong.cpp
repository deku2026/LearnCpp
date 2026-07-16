// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : compare_exchange_weak_strong
// Topic id : part6/f/section02/compare_exchange_weak_strong
// References: C++23 [atomics.types.operations.req]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <numeric>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section02/compare_exchange_weak_strong";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::atomic<int> value{10};
    int expected = 10;
    const bool exchanged = value.compare_exchange_strong(expected, 20);
    LEARN_EXPECT(checks, exchanged);
    LEARN_EXPECT_EQ(checks, value.load(), 20);
    LEARN_EXPECT_EQ(checks, expected, 10);  // unchanged on success

    expected = 999;
    const bool rejected = value.compare_exchange_strong(expected, 30);
    LEARN_EXPECT(checks, !rejected);
    LEARN_EXPECT_EQ(checks, value.load(), 20);
    LEARN_EXPECT_EQ(checks, expected, 20);  // overwritten with the observed value

    constexpr int thread_count = 4;
    constexpr int increments_per_thread = 300;
    std::atomic<int> counter{0};
    std::array<int, thread_count> attempts{};
    std::array<std::thread, thread_count> workers;
    for (int index = 0; index < thread_count; ++index) {
        workers[static_cast<std::size_t>(index)] = std::thread{[&, index] {
            for (int increment = 0; increment < increments_per_thread; ++increment) {
                int observed = counter.load(std::memory_order_relaxed);
                do {
                    ++attempts[static_cast<std::size_t>(index)];
                } while (!counter.compare_exchange_weak(observed, observed + 1, std::memory_order_relaxed,
                                                        std::memory_order_relaxed));
            }
        }};
    }
    for (auto& worker : workers) {
        worker.join();
    }

    const int operation_count = thread_count * increments_per_thread;
    LEARN_EXPECT_EQ(checks, counter.load(std::memory_order_relaxed), operation_count);
    LEARN_EXPECT(checks, std::accumulate(attempts.begin(), attempts.end(), 0) >= operation_count);
    // weak belongs in a retry loop because it may fail spuriously; strong is
    // convenient for a one-shot state transition where spurious failure is not acceptable.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section02/compare_exchange_weak_strong", run>;

}  // namespace
