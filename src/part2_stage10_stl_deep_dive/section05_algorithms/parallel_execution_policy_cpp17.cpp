// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : parallel_execution_policy_cpp17
// Topic id : part2/stage10/section05/parallel_execution_policy_cpp17
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <execution>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/parallel_execution_policy_cpp17";

#if defined(__cpp_lib_parallel_algorithm) && __cpp_lib_parallel_algorithm >= 201603L
static_assert(std::is_execution_policy_v<std::remove_cvref_t<decltype(std::execution::seq)>>);
static_assert(std::is_execution_policy_v<std::remove_cvref_t<decltype(std::execution::par)>>);
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{4, 3, 2, 1};
#if defined(__cpp_lib_parallel_algorithm) && __cpp_lib_parallel_algorithm >= 201603L
    std::sort(std::execution::seq, values.begin(), values.end());
    LEARN_EXPECT(checks, std::is_sorted(values.begin(), values.end()));
    // Work passed to parallel/unsequenced policies must obey their stronger synchronization rules.
    const bool policy_types_are_distinct = !std::same_as<decltype(std::execution::seq), decltype(std::execution::par)>;
    LEARN_EXPECT(checks, policy_types_are_distinct);
    return checks.result();
#else
    // A policy-free algorithm is the semantic baseline when the library has
    // not implemented the C++17 execution-policy overloads.
    std::sort(values.begin(), values.end());
    LEARN_EXPECT(checks, std::is_sorted(values.begin(), values.end()));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return ::learn::ExampleChecks::unavailable(kTopic, "__cpp_lib_parallel_algorithm >= 201603L");
#endif
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section05/parallel_execution_policy_cpp17", run>;

}  // namespace
