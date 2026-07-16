// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : timed_wait_and_predicate
// Topic id : part2/stage11/section04/timed_wait_and_predicate
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section04/timed_wait_and_predicate";

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::mutex mutex;
    std::condition_variable condition;
    bool ready{};
    int payload{};

    std::jthread producer{[&] {
        {
            std::lock_guard lock{mutex};
            payload = 42;
            ready = true;
        }
        condition.notify_one();
    }};

    std::unique_lock lock{mutex};
    const bool completed = condition.wait_for(lock, 2s, [&] { return ready; });
    LEARN_EXPECT(checks, completed);
    LEARN_EXPECT_EQ(checks, payload, 42);
    lock.unlock();
    producer.join();

    std::unique_lock second_lock{mutex};
    const bool impossible = condition.wait_for(second_lock, 1ms, [] { return false; });
    LEARN_EXPECT(checks, !impossible);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section04/timed_wait_and_predicate", run>;

}  // namespace
