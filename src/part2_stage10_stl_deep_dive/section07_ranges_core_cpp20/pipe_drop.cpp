// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_drop
// Topic id : part2/stage10/section07/pipe_drop
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_drop"};
    std::vector values{1, 2, 3, 4, 5};
    int transform_calls = 0;
    auto after_two = values | std::views::transform([&transform_calls](const int value) {
                         ++transform_calls;
                         return value * 10;
                     }) |
                     std::views::drop(2);
    static_assert(std::ranges::sized_range<decltype(after_two)>);

    LEARN_EXPECT_EQ(checks, transform_calls, 0);
    std::vector<int> observed;
    for (const int value : after_two) {
        observed.push_back(value);
    }

    LEARN_EXPECT_EQ(checks, observed, (std::vector{30, 40, 50}));
    LEARN_EXPECT_EQ(checks, transform_calls, 3);
    LEARN_EXPECT_EQ(checks, std::ranges::size(after_two), std::size_t{3});
    LEARN_EXPECT(checks, std::ranges::empty(values | std::views::drop(99)));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_drop", run>;

}  // namespace
