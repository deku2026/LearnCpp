// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : data_race_is_ub_recap
// Topic id : part2/stage11/section03/data_race_is_ub_recap
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <mutex>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/data_race_is_ub_recap";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int protected_value{};
    std::mutex guard;
    std::atomic<int> atomic_value{};
    constexpr int workers = 4;
    constexpr int increments = 200;
    std::array<std::jthread, workers> threads;
    for (auto& thread : threads) {
        thread = std::jthread{[&] {
            for (int index = 0; index < increments; ++index) {
                {
                    std::lock_guard lock{guard};
                    ++protected_value;
                }
                atomic_value.fetch_add(1, std::memory_order_relaxed);
            }
        }};
    }
    for (auto& thread : threads) {
        thread.join();
    }

    LEARN_EXPECT_EQ(checks, protected_value, workers * increments);
    LEARN_EXPECT_EQ(checks, atomic_value.load(), workers * increments);

#if 0
    int raced = 0;
    std::jthread a{[&] { ++raced; }};
    std::jthread b{[&] { ++raced; }};  // Conflicting unsynchronized accesses: UB.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/data_race_is_ub_recap", run>;

}  // namespace
