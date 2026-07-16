// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_join
// Topic id : part2/stage10/section07/pipe_join
//

#include "learn/example_support.hpp"

#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_join"};
    std::vector<std::vector<int>> nested{{1, 2}, {}, {3, 4}};
    auto flattened = nested | std::views::join;
    static_assert(std::ranges::forward_range<decltype(flattened)>);

    std::vector<int> observed;
    for (int& value : flattened) {
        observed.push_back(value);
        value *= 10;
    }
    LEARN_EXPECT_EQ(checks, observed, (std::vector{1, 2, 3, 4}));
    LEARN_EXPECT_EQ(checks, nested, (std::vector<std::vector<int>>{{10, 20}, {}, {30, 40}}));

    auto generated_inner_ranges = std::views::iota(1, 4) |
                                  std::views::transform([](const int value) { return std::vector{value, -value}; }) |
                                  std::views::join;
    static_assert(std::ranges::input_range<decltype(generated_inner_ranges)>);
    static_assert(!std::ranges::forward_range<decltype(generated_inner_ranges)>);
    std::vector<int> generated;
    for (const int value : generated_inner_ranges) {
        generated.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, generated, (std::vector{1, -1, 2, -2, 3, -3}));

#if 0
    // Returning a join view over an lvalue local container would leave a dangling ref_view.
    auto dangling = [] {
        std::vector<std::vector<int>> local{{1}, {2}};
        return local | std::views::join;
    }();
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_join", run>;

}  // namespace
