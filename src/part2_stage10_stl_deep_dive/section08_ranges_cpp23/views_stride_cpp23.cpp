// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_stride_cpp23
// Topic id : part2/stage10/section08/views_stride_cpp23
// Reference: WG21 P1899R3 (views::stride).

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

std::vector<int> collect(auto&& range) {
    return {range.begin(), range.end()};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_stride_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector values{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_stride) && __cpp_lib_ranges_stride >= 202207L
    auto every_third = values | std::views::stride(3);
    LEARN_EXPECT_EQ(checks, collect(every_third), (std::vector{0, 3, 6, 9}));
    LEARN_EXPECT_EQ(checks, every_third.size(), std::size_t{4});
    LEARN_EXPECT(checks, (std::vector<int>{} | std::views::stride(3)).empty());

    values[3] = 30;
    LEARN_EXPECT_EQ(checks, collect(every_third), (std::vector{0, 30, 6, 9}));
    // Count is ceil(N/stride); construction is O(1), iteration skips through the base range.
#if 0
    const auto precondition_violation = values | std::views::stride(0);
#endif
#else
    std::vector<int> modeled;
    for (std::size_t index = 0; index < values.size(); index += 3) {
        modeled.push_back(values[index]);
    }
    LEARN_EXPECT_EQ(checks, modeled, (std::vector{0, 3, 6, 9}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_stride >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_stride_cpp23", run>;

}  // namespace
