// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : borrowed_range_dangling
// Topic id : part2/stage10/section07/borrowed_range_dangling
//

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/borrowed_range_dangling"};
    using TemporaryResult = decltype(std::ranges::find(std::vector{1, 2, 3}, 2));
    using LvalueResult = decltype(std::ranges::find(std::declval<std::vector<int>&>(), 2));

    static_assert(!std::ranges::borrowed_range<std::vector<int>>);
    static_assert(std::ranges::borrowed_range<std::vector<int>&>);
    static_assert(std::ranges::borrowed_range<std::span<int>>);
    static_assert(std::same_as<TemporaryResult, std::ranges::dangling>);
    static_assert(std::same_as<LvalueResult, std::vector<int>::iterator>);

    std::vector values{1, 2, 3};
    const auto from_lvalue = std::ranges::find(values, 2);
    const auto from_temporary_span = std::ranges::find(std::span<int>{values}, 3);
    [[maybe_unused]] const auto protected_result = std::ranges::find(std::vector{4, 5, 6}, 5);

    LEARN_EXPECT(checks, from_lvalue != values.end());
    LEARN_EXPECT_EQ(checks, *from_lvalue, 2);
    LEARN_EXPECT_EQ(checks, *from_temporary_span, 3);
    static_assert(std::same_as<decltype(protected_result), const std::ranges::dangling>);

#if 0
    // `dangling` intentionally has no dereference operation.
    const int unsafe = *std::ranges::find(std::vector{1, 2, 3}, 2);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/borrowed_range_dangling", run>;

}  // namespace
