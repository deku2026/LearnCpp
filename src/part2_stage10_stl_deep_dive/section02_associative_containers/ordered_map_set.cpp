// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : ordered_map_set
// Topic id : part2/stage10/section02/ordered_map_set
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/ordered_map_set";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::map<std::string, int> scores{{"Lin", 8}, {"Ada", 10}};
    const auto [position, inserted] = scores.try_emplace("Ada", 99);
    LEARN_EXPECT(checks, !inserted);
    LEARN_EXPECT_EQ(checks, position->second, 10);
    scores.insert_or_assign("Lin", 9);
    std::vector<std::string> ordered;
    for (const auto& [name, score] : scores) {
        (void)score;
        ordered.push_back(name);
    }
    LEARN_EXPECT_EQ(checks, ordered, std::vector<std::string>({"Ada", "Lin"}));
    const std::set<int> unique{3, 1, 3};
    LEARN_EXPECT_EQ(checks, unique.size(), 2U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section02/ordered_map_set", run>;

}  // namespace
