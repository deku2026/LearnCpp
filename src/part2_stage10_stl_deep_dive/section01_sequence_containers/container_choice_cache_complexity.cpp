// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : container_choice_cache_complexity
// Topic id : part2/stage10/section01/container_choice_cache_complexity
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <deque>
#include <list>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/container_choice_cache_complexity";

static_assert(std::ranges::contiguous_range<std::vector<int>>);
static_assert(std::ranges::random_access_range<std::deque<int>>);
static_assert(!std::ranges::contiguous_range<std::deque<int>>);
static_assert(std::ranges::bidirectional_range<std::list<int>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> dense{1, 2, 3, 4};
    const std::deque<int> double_ended{1, 2, 3, 4};
    const std::list<int> stable_nodes{1, 2, 3, 4};
    LEARN_EXPECT_EQ(checks, std::accumulate(dense.begin(), dense.end(), 0), 10);
    LEARN_EXPECT_EQ(checks, double_ended.front() + double_ended.back(), 5);
    LEARN_EXPECT_EQ(checks, stable_nodes.size(), 4U);
    // Complexity, locality, reference stability and operation mix all matter; no one container is universal.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section01/container_choice_cache_complexity", run>;

}  // namespace
