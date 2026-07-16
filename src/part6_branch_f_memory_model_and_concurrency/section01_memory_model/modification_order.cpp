// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : modification_order
// Topic id : part6/f/section01/modification_order
// References: C++23 [intro.races], [atomics.order]

#include "learn/example_support.hpp"

#include <algorithm>
#include <atomic>
#include <numeric>
#include <ranges>
#include <string_view>
#include <thread>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/modification_order";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr int thread_count = 4;
    constexpr int operations_per_thread = 400;
    constexpr int operation_count = thread_count * operations_per_thread;
    std::atomic<int> sequence{0};
    std::vector<int> tickets(static_cast<std::size_t>(operation_count));
    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    for (int thread_index = 0; thread_index < thread_count; ++thread_index) {
        workers.emplace_back([&, thread_index] {
            const int offset = thread_index * operations_per_thread;
            for (int operation = 0; operation < operations_per_thread; ++operation) {
                // Even relaxed RMWs occupy one total modification order for sequence.
                tickets[static_cast<std::size_t>(offset + operation)] =
                    sequence.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();
    }

    std::ranges::sort(tickets);
    const bool is_complete_order = std::ranges::equal(tickets, std::views::iota(0, operation_count));
    LEARN_EXPECT(checks, is_complete_order);
    LEARN_EXPECT_EQ(checks, sequence.load(std::memory_order_relaxed), operation_count);
    LEARN_EXPECT_EQ(checks, tickets.front(), 0);
    LEARN_EXPECT_EQ(checks, tickets.back(), operation_count - 1);

    // Modification order is per atomic object. Relaxed operations on two
    // different atomics do not create one combined cross-object order.
    std::atomic<int> first{0};
    std::atomic<int> second{0};
    first.store(1, std::memory_order_relaxed);
    second.store(2, std::memory_order_relaxed);
    LEARN_EXPECT_EQ(checks, first.load(std::memory_order_relaxed), 1);
    LEARN_EXPECT_EQ(checks, second.load(std::memory_order_relaxed), 2);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/modification_order", run>;

}  // namespace
