// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : std_print_unicode_thread_safety_cpp23
// Topic id : part2/stage11/section05/std_print_unicode_thread_safety_cpp23
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <string_view>
#include <thread>
#include <version>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#endif

namespace {

constexpr std::string_view kTopic = "part2/stage11/section05/std_print_unicode_thread_safety_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::atomic<int> completed{};
    constexpr int workers = 2;
    std::array<std::jthread, workers> threads;
    for (int index = 0; index < workers; ++index) {
        threads[static_cast<std::size_t>(index)] = std::jthread{[&completed, index] {
            std::print("[std::print worker {}] UTF-8: 你好, C++23!\n", index);
            completed.fetch_add(1, std::memory_order_relaxed);
        }};
    }
    for (auto& thread : threads) {
        thread.join();
    }
    LEARN_EXPECT_EQ(checks, completed.load(), workers);
    // Ordering between calls is intentionally unspecified. Each invocation owns
    // its format arguments, and the implementation synchronizes the stream write.
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "C++23 std::print");
#endif
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage11/section05/std_print_unicode_thread_safety_cpp23", run>;

}  // namespace
