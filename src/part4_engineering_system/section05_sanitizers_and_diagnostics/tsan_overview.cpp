// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : tsan_overview
// Topic id : part4/section05/tsan_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <atomic>
#include <string_view>
#include <thread>

namespace {

#if defined(__clang__)
#if __has_feature(thread_sanitizer)
constexpr bool thread_sanitizer_enabled = true;
#else
constexpr bool thread_sanitizer_enabled = false;
#endif
#elif defined(__SANITIZE_THREAD__)
constexpr bool thread_sanitizer_enabled = true;
#else
constexpr bool thread_sanitizer_enabled = false;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/tsan_overview"};

    std::atomic<int> counter{0};
    {
        std::array<std::jthread, 4> workers;
        for (auto& worker : workers) {
            worker = std::jthread{[&counter] {
                for (int iteration = 0; iteration < 1'000; ++iteration) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            }};
        }
    }  // jthread joins before the final read.
    LEARN_EXPECT_EQ(checks, counter.load(std::memory_order_relaxed), 4'000);

#if 0
    // Dedicated negative test: two threads incrementing this plain int race.
    int racy_counter = 0;
    std::jthread first{[&] { ++racy_counter; }};
    std::jthread second{[&] { ++racy_counter; }};
#endif

    constexpr std::string_view lane = "clang++ -fsanitize=thread -g -O1 -fno-omit-frame-pointer";
    constexpr std::string_view instrumentation =
        thread_sanitizer_enabled ? "TSan instrumentation is active" : "TSan instrumentation is not active";
    LEARN_EXPECT(checks, lane.contains("thread"));
    // TSan gets a separate CI lane; it is not combined with ASan or MSan.
    LEARN_EXPECT(checks, !lane.contains("address"));
    LEARN_EXPECT(checks, instrumentation.starts_with("TSan"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/tsan_overview", run>;

}  // namespace
