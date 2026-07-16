// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_gcd_lcm
// Topic id : part2/stage10/section05/std_gcd_lcm
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <numeric>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_gcd_lcm";

static_assert(std::gcd(48, 18) == 6);
static_assert(std::lcm(6, 15) == 30);
static_assert(std::gcd(-48, 18) == 6);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, std::gcd(0, 12), 12);
    LEARN_EXPECT_EQ(checks, std::lcm(0, 12), 0);
    LEARN_EXPECT_EQ(checks, std::lcm(8, 12), 24);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_gcd_lcm", run>;

}  // namespace
