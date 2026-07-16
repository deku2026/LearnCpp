// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_relaxed
// Topic id : part2/stage11/section03/memory_order_relaxed
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/memory_order_relaxed";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::atomic<int> event_count{};
    constexpr int workers = 3;
    constexpr int events_per_worker = 500;
    std::array<std::jthread, workers> threads;
    for (auto& thread : threads) {
        thread = std::jthread{[&event_count] {
            for (int event = 0; event < events_per_worker; ++event) {
                event_count.fetch_add(1, std::memory_order_relaxed);
            }
        }};
    }
    for (auto& thread : threads) {
        thread.join();
    }

    LEARN_EXPECT_EQ(checks, event_count.load(std::memory_order_relaxed), workers * events_per_worker);
    // Relaxed operations are atomic and participate in modification order, but
    // they do not publish unrelated non-atomic data to another thread.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/memory_order_relaxed", run>;

}  // namespace
