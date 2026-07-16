// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : memory_order_per_hardware_x86_arm
// Topic id : part6/f/section01/memory_order_per_hardware_x86_arm
// References: C++23 [atomics.order], [atomic.fences]

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/memory_order_per_hardware_x86_arm";

void store_relaxed(std::atomic<int>& value, int desired) {
    value.store(desired, std::memory_order_relaxed);
}

void store_release(std::atomic<int>& value, int desired) {
    value.store(desired, std::memory_order_release);
}

void store_seq_cst(std::atomic<int>& value, int desired) {
    value.store(desired, std::memory_order_seq_cst);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::atomic<int> value{0};
    store_relaxed(value, 1);
    LEARN_EXPECT_EQ(checks, value.load(std::memory_order_relaxed), 1);
    store_release(value, 2);
    LEARN_EXPECT_EQ(checks, value.load(std::memory_order_acquire), 2);
    store_seq_cst(value, 3);
    LEARN_EXPECT_EQ(checks, value.load(std::memory_order_seq_cst), 3);

    int payload{};
    std::atomic<bool> ready{false};
    std::thread producer{[&] {
        payload = 42;
        ready.store(true, std::memory_order_release);
        ready.notify_one();
    }};
    std::thread consumer{[&] {
        bool current = ready.load(std::memory_order_acquire);
        while (!current) {
            ready.wait(current, std::memory_order_acquire);
            current = ready.load(std::memory_order_acquire);
        }
        LEARN_EXPECT_EQ(checks, payload, 42);
    }};
    producer.join();
    consumer.join();

    // Inspect the three small store functions in an optimizing compiler:
    // x86 commonly uses a plain store for relaxed/release and a locked exchange
    // for seq_cst; AArch64 distinguishes relaxed str from release stlr. Those
    // mappings are implementation choices—the C++ guarantees above are portable.
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/memory_order_per_hardware_x86_arm", run>;

}  // namespace
