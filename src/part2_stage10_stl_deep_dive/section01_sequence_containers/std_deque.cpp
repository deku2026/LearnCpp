// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_deque
// Topic id : part2/stage10/section01/std_deque
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <deque>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/std_deque";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::deque<int> values{2, 3};
    values.push_front(1);
    values.push_back(4);
    LEARN_EXPECT_EQ(checks, values.front(), 1);
    LEARN_EXPECT_EQ(checks, values.back(), 4);
    values.pop_front();
    values.pop_back();
    LEARN_EXPECT_EQ(checks, values, std::deque<int>({2, 3}));
    LEARN_EXPECT(checks, values[1] == 3);  // Random access, without a contiguous-storage guarantee.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/std_deque", run>;

}  // namespace
