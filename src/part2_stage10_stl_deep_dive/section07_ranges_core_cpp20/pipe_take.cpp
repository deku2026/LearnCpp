// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_take
// Topic id : part2/stage10/section07/pipe_take
//

#include "learn/example_support.hpp"

#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_take"};
    std::vector values{1, 2, 3, 4, 5};
    int transform_calls = 0;
    auto first_three = values | std::views::transform([&transform_calls](const int value) {
                           ++transform_calls;
                           return value * 10;
                       }) |
                       std::views::take(3);
    static_assert(std::ranges::sized_range<decltype(first_three)>);

    LEARN_EXPECT_EQ(checks, transform_calls, 0);
    std::vector<int> observed;
    for (const int value : first_three) {
        observed.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, observed, (std::vector{10, 20, 30}));
    LEARN_EXPECT_EQ(checks, transform_calls, 3);

    std::vector<int> bounded_infinite_source;
    for (const int value : std::views::iota(100) | std::views::take(4)) {
        bounded_infinite_source.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, bounded_infinite_source, (std::vector{100, 101, 102, 103}));
    LEARN_EXPECT(checks, std::ranges::empty(values | std::views::take(0)));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_take", run>;

}  // namespace
