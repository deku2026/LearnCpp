// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : unordered_map_set
// Topic id : part2/stage10/section02/unordered_map_set
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/unordered_map_set";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::unordered_map<std::string, int> counts;
    ++counts["red"];
    ++counts["red"];
    counts.try_emplace("blue", 1);
    LEARN_EXPECT_EQ(checks, counts.at("red"), 2);
    LEARN_EXPECT(checks, counts.contains("blue"));
    const std::unordered_set<int> unique{1, 2, 2};
    LEARN_EXPECT_EQ(checks, unique.size(), 2U);
    // Iteration order is deliberately not asserted: unordered containers do not promise one.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section02/unordered_map_set", run>;

}  // namespace
