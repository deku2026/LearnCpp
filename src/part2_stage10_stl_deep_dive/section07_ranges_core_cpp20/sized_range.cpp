// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : sized_range
// Topic id : part2/stage10/section07/sized_range
//

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <ranges>
#include <sstream>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/sized_range"};
    std::vector values{1, 2, 3, 4, 5};
    int predicate_calls = 0;
    auto odd = values | std::views::filter([&predicate_calls](const int value) {
                   ++predicate_calls;
                   return value % 2 != 0;
               });
    auto first_three = values | std::views::take(3);

    static_assert(std::ranges::sized_range<decltype(values)&>);
    static_assert(std::ranges::sized_range<decltype(first_three)>);
    static_assert(!std::ranges::sized_range<decltype(odd)>);
    static_assert(!std::ranges::sized_range<std::ranges::istream_view<int>>);

    LEARN_EXPECT_EQ(checks, std::ranges::size(values), std::size_t{5});
    LEARN_EXPECT_EQ(checks, std::ranges::size(first_three), std::size_t{3});
    LEARN_EXPECT_EQ(checks, predicate_calls, 0);
    LEARN_EXPECT_EQ(checks, std::ranges::distance(odd), std::ptrdiff_t{3});
    LEARN_EXPECT_EQ(checks, predicate_calls, 5);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/sized_range", run>;

}  // namespace
