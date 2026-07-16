// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : per_container_invalidation_rules
// Topic id : part2/stage10/section04/per_container_invalidation_rules
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <deque>
#include <list>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section04/per_container_invalidation_rules";

struct Rule {
    std::string_view container;
    std::string_view operation;
    bool preserves_other_element_references;
};

constexpr Rule rules[]{
    {"vector", "reallocation", false},
    {"list", "insert", true},
    {"deque", "push at an end", true},  // References remain valid; iterators need not.
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::list<int> nodes{1, 3};
    int& stable = nodes.front();
    nodes.insert(std::next(nodes.begin()), 2);
    LEARN_EXPECT_EQ(checks, stable, 1);
    LEARN_EXPECT(checks, !rules[0].preserves_other_element_references);
    LEARN_EXPECT(checks, rules[1].preserves_other_element_references);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section04/per_container_invalidation_rules", run>;

}  // namespace
