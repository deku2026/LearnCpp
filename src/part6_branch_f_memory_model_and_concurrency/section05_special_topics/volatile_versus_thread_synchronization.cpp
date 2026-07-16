// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : volatile_versus_thread_synchronization
// Topic id : part6/f/section05/volatile_versus_thread_synchronization
// Reference: C++23 working draft [dcl.type.cv], [intro.races], [atomics].

#include "learn/example_support.hpp"

#include <atomic>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section05/volatile_versus_thread_synchronization";
    learn::ExampleChecks checks{topic};

    // A volatile access is an observable side effect for implementation-defined uses such as MMIO.
    // This local is deliberately accessed by one thread only.
    volatile int emulated_device_register = 0;
    emulated_device_register = 7;
    const int register_snapshot = emulated_device_register;
    LEARN_EXPECT_EQ(checks, register_snapshot, 7);

    std::atomic<int> synchronized_counter{};
    constexpr int workers = 4;
    constexpr int iterations = 1'000;
    std::vector<std::thread> threads;
    for (int worker = 0; worker < workers; ++worker) {
        threads.emplace_back([&] {
            for (int index = 0; index < iterations; ++index) {
                synchronized_counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }
    LEARN_EXPECT_EQ(checks, synchronized_counter.load(std::memory_order_relaxed), workers * iterations);

#if 0
    volatile int racing_counter = 0;
    // Concurrent racing_counter++ operations are non-atomic read-modify-writes: data race and UB.
#endif
    // volatile supplies neither atomicity nor inter-thread happens-before. Use atomics, mutexes or
    // higher-level channels; add volatile only when a platform interface specifically requires it.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/volatile_versus_thread_synchronization", run>;

}  // namespace
