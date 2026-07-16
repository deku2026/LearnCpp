// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : range_concept
// Topic id : part2/stage10/section07/range_concept
//

#include "learn/example_support.hpp"

#include <array>
#include <ranges>
#include <sstream>
#include <vector>

namespace {

struct AdlRange {
    std::array<int, 3> values;
};

[[nodiscard]] auto begin(AdlRange& range) noexcept {
    return range.values.begin();
}
[[nodiscard]] auto end(AdlRange& range) noexcept {
    return range.values.end();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/range_concept"};
    static_assert(std::ranges::contiguous_range<std::array<int, 3>&>);
    static_assert(std::ranges::sized_range<std::array<int, 3>&>);
    static_assert(std::ranges::range<AdlRange&>);
    static_assert(std::ranges::input_range<std::ranges::istream_view<int>>);
    static_assert(!std::ranges::forward_range<std::ranges::istream_view<int>>);

    AdlRange custom{{2, 4, 6}};
    int sum = 0;
    for (auto iterator = std::ranges::begin(custom); iterator != std::ranges::end(custom); ++iterator) {
        sum += *iterator;
    }
    LEARN_EXPECT_EQ(checks, sum, 12);

    std::istringstream input{"7 8 9"};
    auto single_pass = std::ranges::istream_view<int>(input);
    std::vector<int> first_traversal;
    for (const int value : single_pass) {
        first_traversal.push_back(value);
    }
    int second_traversal_count = 0;
    for ([[maybe_unused]] const int value : single_pass) {
        ++second_traversal_count;
    }

    LEARN_EXPECT_EQ(checks, first_traversal, (std::vector{7, 8, 9}));
    LEARN_EXPECT_EQ(checks, second_traversal_count, 0);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/range_concept", run>;

}  // namespace
