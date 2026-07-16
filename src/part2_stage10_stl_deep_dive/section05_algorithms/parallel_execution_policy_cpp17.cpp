// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : parallel_execution_policy_cpp17
// Topic id : part2/stage10/section05/parallel_execution_policy_cpp17
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <execution>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/parallel_execution_policy_cpp17";

static_assert(std::is_execution_policy_v<std::remove_cvref_t<decltype(std::execution::seq)>>);
static_assert(std::is_execution_policy_v<std::remove_cvref_t<decltype(std::execution::par)>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{4, 3, 2, 1};
    std::sort(std::execution::seq, values.begin(), values.end());
    LEARN_EXPECT(checks, std::is_sorted(values.begin(), values.end()));
    // Work passed to parallel/unsequenced policies must obey their stronger synchronization rules.
    const bool policy_types_are_distinct = !std::same_as<decltype(std::execution::seq), decltype(std::execution::par)>;
    LEARN_EXPECT(checks, policy_types_are_distinct);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section05/parallel_execution_policy_cpp17", run>;

}  // namespace
