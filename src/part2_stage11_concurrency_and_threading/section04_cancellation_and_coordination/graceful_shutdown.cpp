// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : graceful_shutdown
// Topic id : part2/stage11/section04/graceful_shutdown
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <latch>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section04/graceful_shutdown";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::latch started{1};
    std::atomic<int> iterations{};
    std::jthread worker{[&](std::stop_token stop) {
        started.count_down();
        while (!stop.stop_requested()) {
            const int previous = iterations.fetch_add(1, std::memory_order_relaxed);
            if (previous == 0) {
                iterations.notify_one();
            }
            std::this_thread::yield();
        }
        // Real workers release owned resources and publish final state here.
    }};

    started.wait();
    int current = iterations.load(std::memory_order_relaxed);
    while (current == 0) {
        iterations.wait(current, std::memory_order_relaxed);
        current = iterations.load(std::memory_order_relaxed);
    }
    LEARN_EXPECT(checks, worker.request_stop());
    worker.join();
    LEARN_EXPECT(checks, iterations.load() > 0);
    LEARN_EXPECT(checks, worker.get_stop_token().stop_requested());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section04/graceful_shutdown", run>;

}  // namespace
