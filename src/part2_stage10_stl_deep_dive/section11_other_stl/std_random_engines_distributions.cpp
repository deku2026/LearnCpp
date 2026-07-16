// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_random_engines_distributions
// Topic id : part2/stage10/section11/std_random_engines_distributions
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <array>
#include <random>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/std_random_engines_distributions";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::mt19937 first{123U};
    std::mt19937 second{123U};
    LEARN_EXPECT_EQ(checks, first(), second());  // Equal engine type and seed reproduce the engine sequence.

    std::uniform_int_distribution<int> die{1, 6};
    std::array<int, 6> counts{};
    for (int roll = 0; roll < 600; ++roll) {
        ++counts[static_cast<std::size_t>(die(first) - 1)];
    }
    int total{};
    for (const int count : counts) {
        LEARN_EXPECT(checks, count > 0);
        total += count;
    }
    LEARN_EXPECT_EQ(checks, total, 600);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section11/std_random_engines_distributions", run>;

}  // namespace
