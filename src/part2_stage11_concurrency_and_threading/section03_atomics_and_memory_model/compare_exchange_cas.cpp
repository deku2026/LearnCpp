// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : compare_exchange_cas
// Topic id : part2/stage11/section03/compare_exchange_cas
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/compare_exchange_cas";

void update_max(std::atomic<int>& maximum, int candidate) noexcept {
    int observed = maximum.load(std::memory_order_relaxed);
    while (observed < candidate &&
           !maximum.compare_exchange_weak(observed, candidate, std::memory_order_relaxed, std::memory_order_relaxed)) {
        // On failure, observed is overwritten with the current atomic value.
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::atomic<int> value{10};
    int expected = 8;
    LEARN_EXPECT(checks, !value.compare_exchange_strong(expected, 20));
    LEARN_EXPECT_EQ(checks, expected, 10);
    LEARN_EXPECT_EQ(checks, value.load(), 10);

    expected = 10;
    LEARN_EXPECT(checks, value.compare_exchange_strong(expected, 20));
    LEARN_EXPECT_EQ(checks, value.load(), 20);

    std::atomic<int> maximum{};
    constexpr std::array candidates{4, 19, 7, 42, 11};
    std::array<std::jthread, candidates.size()> workers;
    for (std::size_t index = 0; index < candidates.size(); ++index) {
        workers[index] = std::jthread{[&maximum, candidate = candidates[index]] { update_max(maximum, candidate); }};
    }
    for (auto& worker : workers) {
        worker.join();
    }
    LEARN_EXPECT_EQ(checks, maximum.load(), 42);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/compare_exchange_cas", run>;

}  // namespace
