// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : function_local_static_thread_safety
// Topic id : part6/f/section05/function_local_static_thread_safety
// Reference: C++23 working draft [stmt.dcl], block-variable initialization.

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <thread>
#include <vector>

namespace {

std::atomic<int> construction_count{};

struct LocalService {
    int value;

    explicit LocalService(int initial) : value(initial) { construction_count.fetch_add(1, std::memory_order_relaxed); }
};

const LocalService& service() {
    static const LocalService instance{42};
    return instance;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section05/function_local_static_thread_safety";
    learn::ExampleChecks checks{topic};
    std::array<const LocalService*, 8> results{};
    std::vector<std::thread> threads;
    for (std::size_t index = 0; index < results.size(); ++index) {
        threads.emplace_back([&, index] { results[index] = &service(); });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }

    for (const LocalService* result : results) {
        LEARN_EXPECT_EQ(checks, result, results.front());
        LEARN_EXPECT_EQ(checks, result->value, 42);
    }
    LEARN_EXPECT_EQ(checks, construction_count.load(std::memory_order_relaxed), 1);

    // Since C++11, concurrent first calls wait for one initialization to finish. If construction
    // throws, initialization is retried later; recursive entry during initialization is undefined.
    // Destruction still occurs at program exit, so avoid dependencies on other destroyed statics.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/function_local_static_thread_safety", run>;

}  // namespace
