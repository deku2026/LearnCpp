// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_versus_tree_tradeoff
// Topic id : part2/stage10/section03/flat_versus_tree_tradeoff
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/flat_versus_tree_tradeoff";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::map<int, std::string> tree{{3, "three"}, {1, "one"}, {2, "two"}};
    std::vector<std::pair<int, std::string>> flat{{3, "three"}, {1, "one"}, {2, "two"}};
    std::ranges::sort(flat, {}, &std::pair<int, std::string>::first);
    LEARN_EXPECT_EQ(checks, tree.begin()->first, flat.front().first);
    LEARN_EXPECT_EQ(checks, tree.size(), flat.size());
    // Trees favor stable nodes and logarithmic mutation; flat storage favors locality and bulk/read-heavy use.
    const auto found = std::ranges::lower_bound(flat, 2, {}, &std::pair<int, std::string>::first);
    LEARN_EXPECT_EQ(checks, found->second, std::string{"two"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/flat_versus_tree_tradeoff", run>;

}  // namespace
