// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_acq_rel
// Topic id : part2/stage11/section03/memory_order_acq_rel
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/memory_order_acq_rel";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int payload{};
    int observed{};
    std::atomic<int> state{};

    std::jthread producer{[&] {
        payload = 42;
        state.store(1, std::memory_order_release);
        state.notify_all();
    }};
    std::jthread relay{[&] {
        int expected{};
        do {
            state.wait(expected, std::memory_order_acquire);
            expected = 1;
        } while (!state.compare_exchange_weak(expected, 2, std::memory_order_acq_rel, std::memory_order_acquire));
        state.notify_all();
    }};
    std::jthread consumer{[&] {
        int current = state.load(std::memory_order_acquire);
        while (current != 2) {
            state.wait(current, std::memory_order_acquire);
            current = state.load(std::memory_order_acquire);
        }
        observed = payload;
    }};

    producer.join();
    relay.join();
    consumer.join();
    LEARN_EXPECT_EQ(checks, observed, 42);
    LEARN_EXPECT_EQ(checks, state.load(), 2);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/memory_order_acq_rel", run>;

}  // namespace
