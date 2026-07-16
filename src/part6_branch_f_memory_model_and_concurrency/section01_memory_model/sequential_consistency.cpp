// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : sequential_consistency
// Topic id : part6/f/section01/sequential_consistency
// References: C++23 [intro.races], [atomics.order]

#include "learn/example_support.hpp"

#include <atomic>
#include <barrier>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/sequential_consistency";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // Store-buffering litmus test. In a single total order, both loads cannot
    // precede both stores, so the (0, 0) result is forbidden under seq_cst.
    constexpr int iterations = 512;
    std::atomic<int> x{0};
    std::atomic<int> y{0};
    int left_result{};
    int right_result{};
    std::barrier phase{3};

    std::thread left{[&] {
        for (int iteration = 0; iteration < iterations; ++iteration) {
            phase.arrive_and_wait();
            x.store(1, std::memory_order_seq_cst);
            left_result = y.load(std::memory_order_seq_cst);
            phase.arrive_and_wait();
        }
    }};
    std::thread right{[&] {
        for (int iteration = 0; iteration < iterations; ++iteration) {
            phase.arrive_and_wait();
            y.store(1, std::memory_order_seq_cst);
            right_result = x.load(std::memory_order_seq_cst);
            phase.arrive_and_wait();
        }
    }};

    int both_zero{};
    int observed_iterations{};
    for (int iteration = 0; iteration < iterations; ++iteration) {
        x.store(0, std::memory_order_seq_cst);
        y.store(0, std::memory_order_seq_cst);
        phase.arrive_and_wait();
        phase.arrive_and_wait();
        ++observed_iterations;
        if (left_result == 0 && right_result == 0) {
            ++both_zero;
        }
    }
    left.join();
    right.join();

    LEARN_EXPECT_EQ(checks, observed_iterations, iterations);
    LEARN_EXPECT_EQ(checks, both_zero, 0);
    LEARN_EXPECT(checks, x.load() == 1 || y.load() == 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/sequential_consistency", run>;

}  // namespace
