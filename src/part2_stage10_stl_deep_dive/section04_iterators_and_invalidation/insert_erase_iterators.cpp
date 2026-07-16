// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : insert_erase_iterators
// Topic id : part2/stage10/section04/insert_erase_iterators
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <iterator>
#include <list>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section04/insert_erase_iterators";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> source{1, 2, 3};
    std::vector<int> appended{0};
    std::ranges::copy(source, std::back_inserter(appended));
    LEARN_EXPECT_EQ(checks, appended, std::vector<int>({0, 1, 2, 3}));

    std::list<int> front{4};
    std::ranges::copy(source, std::front_inserter(front));
    LEARN_EXPECT_EQ(checks, front, std::list<int>({3, 2, 1, 4}));

    auto new_end = std::remove(appended.begin(), appended.end(), 2);
    appended.erase(new_end, appended.end());  // Classic erase-remove pairs a logical and physical erase.
    LEARN_EXPECT_EQ(checks, appended, std::vector<int>({0, 1, 3}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section04/insert_erase_iterators", run>;

}  // namespace
