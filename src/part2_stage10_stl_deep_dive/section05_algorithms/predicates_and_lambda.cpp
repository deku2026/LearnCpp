// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : predicates_and_lambda
// Topic id : part2/stage10/section05/predicates_and_lambda
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/predicates_and_lambda";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 4, 7, 10};
    int threshold = 5;
    const auto above = [threshold](int value) { return value > threshold; };
    LEARN_EXPECT_EQ(checks, std::ranges::count_if(values, above), 2);
    LEARN_EXPECT(checks, std::ranges::any_of(values, [](int value) { return value % 2 == 0; }));
    LEARN_EXPECT(checks, std::ranges::none_of(values, [](int value) { return value < 0; }));
    LEARN_EXPECT(checks, !std::ranges::all_of(values, above));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/predicates_and_lambda", run>;

}  // namespace
