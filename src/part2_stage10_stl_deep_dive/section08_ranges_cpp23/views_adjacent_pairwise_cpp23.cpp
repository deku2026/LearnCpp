// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_adjacent_pairwise_cpp23
// Topic id : part2/stage10/section08/views_adjacent_pairwise_cpp23
// Reference: WG21 P2321R2 (__cpp_lib_ranges_zip also covers adjacent/pairwise views).

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

#include <functional>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_adjacent_pairwise_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector values{1, 4, 9, 16};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    auto pairs = values | std::views::pairwise;
    std::vector<int> differences;
    for (auto [left, right] : pairs) {
        differences.push_back(right - left);
    }
    LEARN_EXPECT_EQ(checks, differences, (std::vector{3, 5, 7}));

    auto sums = values | std::views::adjacent_transform<2>(std::plus<>{});
    LEARN_EXPECT_EQ(checks, (std::vector<int>{sums.begin(), sums.end()}), (std::vector{5, 13, 25}));
    LEARN_EXPECT(checks, (std::vector{42} | std::views::pairwise).empty());

    values[1] = 5;
    LEARN_EXPECT_EQ(checks, std::get<1>(*pairs.begin()), 5);
    // For width K, output size is max(N-K+1, 0); construction is lazy and O(1).
#else
    std::vector<int> modeled;
    for (std::size_t index = 1; index < values.size(); ++index) {
        modeled.push_back(values[index] - values[index - 1]);
    }
    LEARN_EXPECT_EQ(checks, modeled, (std::vector{3, 5, 7}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_zip >= 202110L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_adjacent_pairwise_cpp23", run>;

}  // namespace
