// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : relaxed_semantics
// Topic id : part6/f/section01/relaxed_semantics
// References: C++23 [atomics.order], relaxed ordering

#include "learn/example_support.hpp"

#include <algorithm>
#include <atomic>
#include <numeric>
#include <string_view>
#include <thread>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/relaxed_semantics";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr int thread_count = 4;
    constexpr int entries_per_thread = 256;
    constexpr std::size_t entry_count = thread_count * entries_per_thread;
    std::atomic<std::size_t> next_slot{0};
    std::atomic<int> completed{0};
    std::vector<int> slots(entry_count, 0);
    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    for (int thread_index = 0; thread_index < thread_count; ++thread_index) {
        workers.emplace_back([&, thread_index] {
            for (int entry = 0; entry < entries_per_thread; ++entry) {
                // Atomicity alone is enough to hand out distinct indices.
                const std::size_t slot = next_slot.fetch_add(1, std::memory_order_relaxed);
                slots[slot] = thread_index + 1;  // each thread owns a distinct element
                completed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();  // join publishes every slot write to this thread
    }

    LEARN_EXPECT_EQ(checks, next_slot.load(std::memory_order_relaxed), entry_count);
    LEARN_EXPECT_EQ(checks, completed.load(std::memory_order_relaxed), static_cast<int>(entry_count));
    LEARN_EXPECT(checks, std::ranges::all_of(slots, [](int value) { return value != 0; }));
    const int expected_sum = entries_per_thread * (1 + 2 + 3 + 4);
    LEARN_EXPECT_EQ(checks, std::accumulate(slots.begin(), slots.end(), 0), expected_sum);

#if 0
    // Inactive negative form: relaxed does not publish ordinary payload data.
    int payload = 0;
    std::atomic<bool> ready = false;
    std::thread producer([&] {
        payload = 42;
        ready.store(true, std::memory_order_relaxed);
    });
    std::thread consumer([&] {
        while (!ready.load(std::memory_order_relaxed)) {}
        use(payload); // no happens-before: data race/UB
    });
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/relaxed_semantics", run>;

}  // namespace
