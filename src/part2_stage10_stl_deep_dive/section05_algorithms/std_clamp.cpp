// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_clamp
// Topic id : part2/stage10/section05/std_clamp
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_clamp";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, std::clamp(5, 0, 10), 5);
    LEARN_EXPECT_EQ(checks, std::clamp(-2, 0, 10), 0);
    LEARN_EXPECT_EQ(checks, std::clamp(20, 0, 10), 10);
    const std::string low = "a";
    const std::string high = "m";
    const std::string value = "z";
    LEARN_EXPECT_EQ(checks, std::clamp(value, low, high), high);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_clamp", run>;

}  // namespace
