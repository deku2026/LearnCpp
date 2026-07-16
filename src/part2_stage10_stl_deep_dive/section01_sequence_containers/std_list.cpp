// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_list
// Topic id : part2/stage10/section01/std_list
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <list>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/std_list";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::list<int> left{1, 4};
    std::list<int> right{2, 3};
    auto moved = right.begin();
    const int* address = &*moved;
    left.splice(std::next(left.begin()), right, moved);
    LEARN_EXPECT_EQ(checks, left, std::list<int>({1, 2, 4}));
    LEARN_EXPECT_EQ(checks, right, std::list<int>({3}));
    LEARN_EXPECT(checks, &*std::next(left.begin()) == address);  // Splice relinks the existing node.
    left.sort();
    LEARN_EXPECT_EQ(checks, left.back(), 4);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/std_list", run>;

}  // namespace
