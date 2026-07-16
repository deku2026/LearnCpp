// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : tsan_lane
// Topic id : part4/section06/tsan_lane
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <compare>
#include <ranges>
#include <string_view>
#include <thread>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section06/tsan_lane";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::atomic<int> counter{};
    std::vector<std::jthread> workers;
    for (int index = 0; index < 2; ++index) {
        workers.emplace_back([&] {
            for (int turn = 0; turn < 500; ++turn) counter.fetch_add(1, std::memory_order_relaxed);
        });
    }
    workers.clear();
    LEARN_EXPECT_EQ(checks, counter.load(std::memory_order_relaxed), 1'000);
    const std::array<std::string_view, 3> isolation{"separate sanitizer build", "bounded concurrent tests",
                                                    "no ASan mixing"};
    LEARN_EXPECT_EQ(checks, isolation.size(), 3U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section06/tsan_lane", run>;

}  // namespace
