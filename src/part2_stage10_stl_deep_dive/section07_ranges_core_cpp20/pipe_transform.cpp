// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_transform
// Topic id : part2/stage10/section07/pipe_transform
//

#include "learn/example_support.hpp"

#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_transform"};
    std::vector values{1, 2, 3, 4};
    int transform_calls = 0;
    auto squares = values | std::views::transform([&transform_calls](const int value) {
                       ++transform_calls;
                       return value * value;
                   });
    static_assert(std::is_same_v<std::ranges::range_reference_t<decltype(squares)>, int>);

    LEARN_EXPECT_EQ(checks, transform_calls, 0);
    std::vector<int> observed;
    for (const int value : squares) {
        observed.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, observed, (std::vector{1, 4, 9, 16}));
    LEARN_EXPECT_EQ(checks, transform_calls, 4);

    int second_sum = 0;
    for (const int value : squares) {
        second_sum += value;
    }
    LEARN_EXPECT_EQ(checks, second_sum, 30);
    LEARN_EXPECT_EQ(checks, transform_calls, 8);  // Views do not memoize transformed values.

    auto references = values | std::views::transform([](int& value) -> int& { return value; });
    static_assert(std::is_same_v<std::ranges::range_reference_t<decltype(references)>, int&>);
    *references.begin() = 10;
    LEARN_EXPECT_EQ(checks, values.front(), 10);

#if 0
    auto dangling_callable = [] {
        int factor = 2;
        return std::views::iota(1, 4) |
               std::views::transform([&factor](const int value) { return value * factor; });
    }();  // The lazy adaptor stored a reference to a destroyed local.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_transform", run>;

}  // namespace
