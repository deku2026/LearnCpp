// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section03_atomic_helpers
// Item     : atomic_flag
// Topic id : part6/f/section03/atomic_flag
// Reference: C++23 working draft [atomics.flag], [atomics.wait].

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace {

class FlagMutex {
public:
    void lock() noexcept {
        while (locked_.test_and_set(std::memory_order_acquire)) {
#if defined(__cpp_lib_atomic_wait) && __cpp_lib_atomic_wait >= 201907L
            locked_.wait(true, std::memory_order_relaxed);
#else
            std::this_thread::yield();
#endif
        }
    }

    void unlock() noexcept {
        locked_.clear(std::memory_order_release);
#if defined(__cpp_lib_atomic_wait) && __cpp_lib_atomic_wait >= 201907L
        locked_.notify_one();
#endif
    }

private:
    std::atomic_flag locked_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section03/atomic_flag";
    learn::ExampleChecks checks{topic};

    std::atomic_flag probe{};
    LEARN_EXPECT(checks, !probe.test_and_set(std::memory_order_acquire));
    LEARN_EXPECT(checks, probe.test_and_set(std::memory_order_relaxed));
    probe.clear(std::memory_order_release);
    LEARN_EXPECT(checks, !probe.test_and_set(std::memory_order_acquire));
    probe.clear(std::memory_order_release);

    FlagMutex mutex;
    int protected_counter{};
    constexpr int workers = 4;
    constexpr int iterations = 1'000;
    std::vector<std::thread> threads;
    for (int worker = 0; worker < workers; ++worker) {
        threads.emplace_back([&] {
            for (int index = 0; index < iterations; ++index) {
                const std::lock_guard lock{mutex};
                ++protected_counter;
            }
        });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }
    LEARN_EXPECT_EQ(checks, protected_counter, workers * iterations);

    // atomic_flag is the minimal lock-free atomic type. This mutex is non-recursive and deliberately
    // tiny; real code normally prefers std::mutex, whose blocking and fairness strategy is tuned by
    // the library rather than assuming that spinning is always productive.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section03/atomic_flag", run>;

}  // namespace
