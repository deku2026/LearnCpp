// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_forward_list
// Topic id : part2/stage10/section01/std_forward_list
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <forward_list>
#include <iterator>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/std_forward_list";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::forward_list<int> values{2, 3};
    values.insert_after(values.before_begin(), 1);
    auto second = std::next(values.begin());
    values.erase_after(second);  // Removes 3; singly linked operations name the predecessor.
    values.push_front(0);
    LEARN_EXPECT_EQ(checks, std::distance(values.begin(), values.end()), 3);
    LEARN_EXPECT_EQ(checks, values.front(), 0);
    LEARN_EXPECT_EQ(checks, *std::next(values.begin(), 2), 2);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/std_forward_list", run>;

}  // namespace
