// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : cas_loop
// Topic id : part6/f/section02/cas_loop
// References: C++23 [atomics.types.operations.req]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <numeric>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section02/cas_loop";

bool update_max(std::atomic<int>& maximum, int candidate, int& attempts) {
    int observed = maximum.load(std::memory_order_relaxed);
    while (observed < candidate) {
        ++attempts;
        if (maximum.compare_exchange_weak(observed, candidate, std::memory_order_relaxed, std::memory_order_relaxed)) {
            return true;
        }
        // Failure writes the current maximum into observed; reconsider the
        // loop condition before retrying because another thread may have won.
    }
    return false;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr int thread_count = 4;
    constexpr int candidates_per_thread = 100;
    std::atomic<int> maximum{-1};
    std::array<int, thread_count> attempts{};
    std::array<int, thread_count> successful_updates{};
    std::array<std::thread, thread_count> workers;

    for (int thread_index = 0; thread_index < thread_count; ++thread_index) {
        workers[static_cast<std::size_t>(thread_index)] = std::thread{[&, thread_index] {
            for (int offset = 0; offset < candidates_per_thread; ++offset) {
                const int candidate = offset * thread_count + thread_index;
                if (update_max(maximum, candidate, attempts[static_cast<std::size_t>(thread_index)])) {
                    ++successful_updates[static_cast<std::size_t>(thread_index)];
                }
            }
        }};
    }
    for (auto& worker : workers) {
        worker.join();
    }

    const int expected_maximum = thread_count * candidates_per_thread - 1;
    LEARN_EXPECT_EQ(checks, maximum.load(std::memory_order_relaxed), expected_maximum);
    LEARN_EXPECT(checks, std::accumulate(successful_updates.begin(), successful_updates.end(), 0) > 0);
    LEARN_EXPECT(checks, std::accumulate(attempts.begin(), attempts.end(), 0) >=
                             std::accumulate(successful_updates.begin(), successful_updates.end(), 0));

    // This numeric reduction needs atomicity but publishes no payload, so
    // relaxed is sufficient. A CAS that publishes/consumes data would need
    // release/acquire (or acq_rel) on success and a legal load order on failure.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section02/cas_loop", run>;

}  // namespace
