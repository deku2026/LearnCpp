// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_seq_cst
// Topic id : part2/stage11/section03/memory_order_seq_cst
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <barrier>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/memory_order_seq_cst";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::atomic<bool> first_written{false};
    std::atomic<bool> second_written{false};
    bool first_observed{};
    bool second_observed{};
    std::barrier start{3};

    std::jthread first{[&] {
        start.arrive_and_wait();
        first_written.store(true, std::memory_order_seq_cst);
        first_observed = second_written.load(std::memory_order_seq_cst);
    }};
    std::jthread second{[&] {
        start.arrive_and_wait();
        second_written.store(true, std::memory_order_seq_cst);
        second_observed = first_written.load(std::memory_order_seq_cst);
    }};
    start.arrive_and_wait();
    first.join();
    second.join();

    // All seq_cst operations fit one total order consistent with each thread;
    // both loads therefore cannot precede both stores.
    LEARN_EXPECT(checks, first_observed || second_observed);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/memory_order_seq_cst", run>;

}  // namespace
