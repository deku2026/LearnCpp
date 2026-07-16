// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : atomic_ref_cpp20
// Topic id : part2/stage11/section03/atomic_ref_cpp20
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/atomic_ref_cpp20";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    alignas(std::atomic_ref<int>::required_alignment) int ordinary_value{};
    constexpr int workers = 4;
    constexpr int increments = 250;
    std::array<std::jthread, workers> threads;
    for (auto& thread : threads) {
        thread = std::jthread{[&ordinary_value] {
            std::atomic_ref reference{ordinary_value};
            for (int index = 0; index < increments; ++index) {
                reference.fetch_add(1, std::memory_order_relaxed);
            }
        }};
    }
    for (auto& thread : threads) {
        thread.join();
    }

    LEARN_EXPECT_EQ(checks, ordinary_value, workers * increments);
    std::atomic_ref reference{ordinary_value};
    LEARN_EXPECT_EQ(checks, reference.exchange(7), workers * increments);
    LEARN_EXPECT_EQ(checks, ordinary_value, 7);

    // While any atomic_ref exists, every concurrent access to the referenced
    // object must also be atomic, and the object must satisfy required_alignment.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/atomic_ref_cpp20", run>;

}  // namespace
