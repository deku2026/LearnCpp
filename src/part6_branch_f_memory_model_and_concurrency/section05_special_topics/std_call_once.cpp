// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : std_call_once
// Topic id : part6/f/section05/std_call_once
// Reference: C++23 working draft [thread.once.callonce].

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section05/std_call_once";
    learn::ExampleChecks checks{topic};
    std::once_flag flag;
    std::atomic<int> attempts{};
    int initialized_value{};
    std::array<int, 6> observations{};

    auto initialize = [&] {
        const int attempt = attempts.fetch_add(1, std::memory_order_relaxed) + 1;
        if (attempt == 1) {
            throw std::runtime_error{"retry initialization"};
        }
        initialized_value = 42;
    };

    std::vector<std::thread> threads;
    for (std::size_t index = 0; index < observations.size(); ++index) {
        threads.emplace_back([&, index] {
            try {
                std::call_once(flag, initialize);
                observations[index] = initialized_value;
            } catch (const std::runtime_error&) {
                observations[index] = -1;
            }
        });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }

    LEARN_EXPECT_EQ(checks, attempts.load(std::memory_order_relaxed), 2);
    LEARN_EXPECT_EQ(checks, std::ranges::count(observations, -1), std::ptrdiff_t{1});
    LEARN_EXPECT_EQ(checks, std::ranges::count(observations, 42), std::ptrdiff_t{5});

    // An exceptional active call does not consume the flag; a later call retries. The successful
    // return synchronizes with every passive return, so initialized_value is safe to read there.
    // once_flag is deliberately non-copyable and non-resettable.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/std_call_once", run>;

}  // namespace
