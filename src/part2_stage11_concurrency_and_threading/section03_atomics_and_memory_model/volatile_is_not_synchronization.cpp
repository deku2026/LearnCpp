// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : volatile_is_not_synchronization
// Topic id : part2/stage11/section03/volatile_is_not_synchronization
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <thread>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/volatile_is_not_synchronization";

static_assert(!std::is_same_v<volatile int, std::atomic<int>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    volatile int device_like_register{};
    device_like_register = 7;
    LEARN_EXPECT_EQ(checks, device_like_register, 7);

    int payload{};
    std::atomic<bool> ready{false};
    std::jthread producer{[&] {
        payload = 42;
        ready.store(true, std::memory_order_release);
        ready.notify_one();
    }};
    ready.wait(false, std::memory_order_acquire);
    LEARN_EXPECT_EQ(checks, payload, 42);
    producer.join();

#if 0
    volatile bool bad_ready = false;
    // Reading/writing bad_ready and an ordinary payload from two threads still
    // forms data races. volatile controls observable accesses, not synchronization.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage11/section03/volatile_is_not_synchronization", run>;

}  // namespace
