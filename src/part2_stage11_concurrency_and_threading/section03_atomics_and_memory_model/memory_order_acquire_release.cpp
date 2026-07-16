// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_acquire_release
// Topic id : part2/stage11/section03/memory_order_acquire_release
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <string>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/memory_order_acquire_release";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::string payload;
    std::atomic<bool> ready{false};
    int observed_size{};

    std::jthread consumer{[&] {
        ready.wait(false, std::memory_order_acquire);
        // The acquire load that observes the release store makes the preceding
        // payload write visible; this read is therefore not a data race.
        observed_size = static_cast<int>(payload.size());
    }};
    std::jthread producer{[&] {
        payload = "published";
        ready.store(true, std::memory_order_release);
        ready.notify_one();
    }};

    producer.join();
    consumer.join();
    LEARN_EXPECT_EQ(checks, observed_size, 9);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/memory_order_acquire_release", run>;

}  // namespace
