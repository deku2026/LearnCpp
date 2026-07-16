// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section09_customization_points
// Item     : range_v3_origin
// Topic id : part2/stage10/section09/range_v3_origin
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section09/range_v3_origin";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3, 4, 5};
    auto pipeline = values | std::views::filter([](int value) { return value % 2 != 0; }) |
                    std::views::transform([](int value) { return value * value; });
    std::vector<int> result;
    for (const int value : pipeline) {
        result.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, result, std::vector<int>({1, 9, 25}));
    static_assert(std::ranges::view<decltype(pipeline)>);
    // The range-v3 project incubated many ideas; this topic uses only their standardized descendants.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section09/range_v3_origin", run>;

}  // namespace
