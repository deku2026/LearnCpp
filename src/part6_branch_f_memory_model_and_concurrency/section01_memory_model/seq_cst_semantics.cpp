// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : seq_cst_semantics
// Topic id : part6/f/section01/seq_cst_semantics
// References: C++23 [atomics.order], sequentially consistent ordering

#include "learn/example_support.hpp"

#include <atomic>
#include <barrier>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/seq_cst_semantics";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // IRIW (independent reads of independent writes). A single seq_cst order
    // forbids two readers from disagreeing in the crossed way checked below.
    constexpr int iterations = 384;
    std::atomic<int> x{0};
    std::atomic<int> y{0};
    int first_x{};
    int first_y{};
    int second_y{};
    int second_x{};
    std::barrier phase{5};

    std::thread x_writer{[&] {
        for (int iteration = 0; iteration < iterations; ++iteration) {
            phase.arrive_and_wait();
            x.store(1);  // The default order is seq_cst.
            phase.arrive_and_wait();
        }
    }};
    std::thread y_writer{[&] {
        for (int iteration = 0; iteration < iterations; ++iteration) {
            phase.arrive_and_wait();
            y.store(1);
            phase.arrive_and_wait();
        }
    }};
    std::thread first_reader{[&] {
        for (int iteration = 0; iteration < iterations; ++iteration) {
            phase.arrive_and_wait();
            first_x = x.load();
            first_y = y.load();
            phase.arrive_and_wait();
        }
    }};
    std::thread second_reader{[&] {
        for (int iteration = 0; iteration < iterations; ++iteration) {
            phase.arrive_and_wait();
            second_y = y.load();
            second_x = x.load();
            phase.arrive_and_wait();
        }
    }};

    int inconsistent_views{};
    for (int iteration = 0; iteration < iterations; ++iteration) {
        x.store(0);
        y.store(0);
        phase.arrive_and_wait();
        phase.arrive_and_wait();
        if (first_x == 1 && first_y == 0 && second_y == 1 && second_x == 0) {
            ++inconsistent_views;
        }
    }
    x_writer.join();
    y_writer.join();
    first_reader.join();
    second_reader.join();

    LEARN_EXPECT_EQ(checks, inconsistent_views, 0);
    LEARN_EXPECT_EQ(checks, x.load(), 1);
    LEARN_EXPECT_EQ(checks, y.load(), 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/seq_cst_semantics", run>;

}  // namespace
