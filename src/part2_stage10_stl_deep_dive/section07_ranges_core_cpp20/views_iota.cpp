// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_iota
// Topic id : part2/stage10/section07/views_iota
//

#include "learn/example_support.hpp"

#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/views_iota"};
    auto bounded = std::views::iota(3, 7);
    auto unbounded = std::views::iota(10);
    static_assert(std::ranges::random_access_range<decltype(bounded)>);
    static_assert(std::ranges::sized_range<decltype(bounded)>);
    static_assert(std::ranges::common_range<decltype(bounded)>);
    static_assert(std::ranges::borrowed_range<decltype(bounded)>);
    static_assert(!std::ranges::sized_range<decltype(unbounded)>);
    static_assert(!std::ranges::common_range<decltype(unbounded)>);

    std::vector<int> finite_values;
    for (const int value : bounded) {
        finite_values.push_back(value);
    }

    std::vector<int> first_four;
    for (const int value : unbounded | std::views::take(4)) {
        first_four.push_back(value);
    }

    std::vector<int> squares;
    for (const int square :
         std::views::iota(1, 6) | std::views::transform([](const int value) { return value * value; })) {
        squares.push_back(square);
    }

    LEARN_EXPECT_EQ(checks, finite_values, (std::vector{3, 4, 5, 6}));
    LEARN_EXPECT_EQ(checks, first_four, (std::vector{10, 11, 12, 13}));
    LEARN_EXPECT_EQ(checks, squares, (std::vector{1, 4, 9, 16, 25}));

#if 0
    // Never consume an unbounded iota without a terminating adaptor or condition.
    for (const int value : std::views::iota(0)) {
        use(value);
    }
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_iota", run>;

}  // namespace
