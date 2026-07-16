// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section04_features_by_standard_cpp20
// Item     : cpp20_library_features_overview
// Topic id : part2/stage14/section04/cpp20_library_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <bit>
#include <numbers>
#include <ranges>
#include <span>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section04/cpp20_library_features_overview";

constexpr std::array<std::string_view, 12> features{
    "ranges",
    "span",
    "format",
    "jthread/stop_token",
    "latch/barrier/semaphore",
    "atomic wait",
    "calendar/time-zone",
    "bit operations",
    "numbers",
    "source_location",
    "syncstream",
    "make_shared arrays",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3, 4};
    const std::span view{values};
    auto even = view | std::views::filter([](int value) { return value % 2 == 0; });
    LEARN_EXPECT_EQ(checks, std::ranges::distance(even), 2);
    LEARN_EXPECT_EQ(checks, std::popcount(0b1011U), 3);
    LEARN_EXPECT(checks, std::numbers::pi > 3.14);
    LEARN_EXPECT_EQ(checks, features.size(), 12U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section04/cpp20_library_features_overview", run>;

}  // namespace
