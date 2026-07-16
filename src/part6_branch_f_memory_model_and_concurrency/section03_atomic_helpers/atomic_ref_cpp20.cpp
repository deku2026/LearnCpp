// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section03_atomic_helpers
// Item     : atomic_ref_cpp20
// Topic id : part6/f/section03/atomic_ref_cpp20
// Reference: C++23 working draft [atomics.ref]; __cpp_lib_atomic_ref.

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section03/atomic_ref_cpp20";
    learn::ExampleChecks checks{topic};

#if defined(__cpp_lib_atomic_ref) && __cpp_lib_atomic_ref >= 201806L
    static_assert(std::is_trivially_copyable_v<int>);
    alignas(std::atomic_ref<int>::required_alignment) int counter{};
    int observed{};
    {
        std::atomic_ref<int> atomic_counter{counter};
        constexpr int workers = 4;
        constexpr int iterations = 1'000;
        std::vector<std::thread> threads;
        for (int worker = 0; worker < workers; ++worker) {
            threads.emplace_back([&] {
                for (int index = 0; index < iterations; ++index) {
                    atomic_counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        for (std::thread& thread : threads) {
            thread.join();
        }
        observed = atomic_counter.load(std::memory_order_relaxed);
    }
    LEARN_EXPECT_EQ(checks, observed, 4'000);
    LEARN_EXPECT_EQ(checks, counter, 4'000);
    LEARN_EXPECT_EQ(checks, reinterpret_cast<std::uintptr_t>(&counter) % std::atomic_ref<int>::required_alignment,
                    std::uintptr_t{0});

    // Every potentially concurrent access must use atomic_ref while any atomic_ref to the object
    // exists. The referenced object must outlive the wrappers and meet required_alignment.
    return checks.result();
#else
    std::atomic<int> counter{};
    counter.fetch_add(1, std::memory_order_relaxed);
    LEARN_EXPECT_EQ(checks, counter.load(std::memory_order_relaxed), 1);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_atomic_ref >= 201806L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section03/atomic_ref_cpp20", run>;

}  // namespace
