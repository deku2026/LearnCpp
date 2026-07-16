// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_chunk_by_cpp23
// Topic id : part2/stage10/section08/views_chunk_by_cpp23
// Reference: WG21 P2443R1 (views::chunk_by).

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

std::vector<std::vector<int>> collect_groups(auto&& groups) {
    std::vector<std::vector<int>> result;
    for (auto group : groups) {
        result.emplace_back(group.begin(), group.end());
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_chunk_by_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector values{1, 2, 2, 3, 1, 2};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_chunk_by) && __cpp_lib_ranges_chunk_by >= 202202L
    auto nondecreasing_runs = values | std::views::chunk_by(std::ranges::less_equal{});
    LEARN_EXPECT_EQ(checks, collect_groups(nondecreasing_runs), (std::vector<std::vector<int>>{{1, 2, 2, 3}, {1, 2}}));

    values[4] = 4;
    values[5] = 5;
    auto refreshed_runs = values | std::views::chunk_by(std::ranges::less_equal{});
    LEARN_EXPECT_EQ(checks, collect_groups(refreshed_runs).size(), std::size_t{1});
    LEARN_EXPECT(checks, (std::vector<int>{} | std::views::chunk_by(std::ranges::equal_to{})).empty());
    // The predicate compares adjacent elements only; N elements require at most N-1 calls. A view may cache a
    // discovered boundary, so rebuild it after value mutations that change the partition.
#else
    std::vector<std::vector<int>> modeled{{values.front()}};
    for (std::size_t index = 1; index < values.size(); ++index) {
        if (values[index - 1] <= values[index]) {
            modeled.back().push_back(values[index]);
        } else {
            modeled.push_back({values[index]});
        }
    }
    LEARN_EXPECT_EQ(checks, modeled.size(), std::size_t{2});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_chunk_by >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_chunk_by_cpp23", run>;

}  // namespace
