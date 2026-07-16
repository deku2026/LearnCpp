// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : multimap_multiset
// Topic id : part2/stage10/section02/multimap_multiset
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/multimap_multiset";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::multimap<std::string, int> grades{{"Ada", 90}, {"Ada", 95}, {"Lin", 88}};
    const auto [first, last] = grades.equal_range("Ada");
    std::vector<int> ada;
    for (auto iterator = first; iterator != last; ++iterator) {
        ada.push_back(iterator->second);
    }
    LEARN_EXPECT_EQ(checks, ada, std::vector<int>({90, 95}));
    std::multiset<int> values{2, 1, 2};
    LEARN_EXPECT_EQ(checks, values.count(2), 2U);
    LEARN_EXPECT_EQ(checks, values.erase(2), 2U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section02/multimap_multiset", run>;

}  // namespace
