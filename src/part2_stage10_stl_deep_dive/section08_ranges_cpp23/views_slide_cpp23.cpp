// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_slide_cpp23
// Topic id : part2/stage10/section08/views_slide_cpp23
// Reference: WG21 P2442R1 (views::chunk and views::slide).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<ranges>)
#include <ranges>
#define LEARN_HAS_RANGES_HEADER 1
#else
#define LEARN_HAS_RANGES_HEADER 0
#endif

#include <vector>

namespace {

std::vector<std::vector<int>> collect_windows(auto&& windows) {
    std::vector<std::vector<int>> result;
    for (auto window : windows) {
        result.emplace_back(window.begin(), window.end());
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_slide_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector values{1, 2, 3, 4, 5};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_slide) && __cpp_lib_ranges_slide >= 202202L
    auto windows = values | std::views::slide(3);
    LEARN_EXPECT_EQ(checks, collect_windows(windows), (std::vector<std::vector<int>>{{1, 2, 3}, {2, 3, 4}, {3, 4, 5}}));
    LEARN_EXPECT_EQ(checks, windows.size(), std::size_t{3});
    LEARN_EXPECT(checks, (values | std::views::slide(6)).empty());

    values[2] = 30;
    LEARN_EXPECT_EQ(checks, collect_windows(windows).front(), (std::vector{1, 2, 30}));
    // Count is max(N-width+1, 0). Each window aliases the source and overlaps its neighbors.
#if 0
    const auto precondition_violation = values | std::views::slide(0);
#endif
#else
    std::vector<std::vector<int>> modeled;
    for (std::size_t first = 0; first + 3 <= values.size(); ++first) {
        modeled.emplace_back(values.begin() + static_cast<std::ptrdiff_t>(first),
                             values.begin() + static_cast<std::ptrdiff_t>(first + 3));
    }
    LEARN_EXPECT_EQ(checks, modeled.size(), std::size_t{3});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_slide >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_slide_cpp23", run>;

}  // namespace
