// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_shift_left_right_cpp23
// Topic id : part2/stage10/section08/ranges_shift_left_right_cpp23
// Reference: WG21 P2440R1; the feature-test macro is __cpp_lib_shift, not ranges_shift.

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<algorithm>)
#include <algorithm>
#define LEARN_HAS_ALGORITHM_HEADER 1
#else
#define LEARN_HAS_ALGORITHM_HEADER 0
#endif

#include <vector>

namespace {

std::vector<int> copy_subrange(auto&& range) {
    return {range.begin(), range.end()};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_shift_left_right_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_shift) && __cpp_lib_shift >= 202202L
    std::vector left{1, 2, 3, 4, 5};
    const auto left_valid = std::ranges::shift_left(left, 2);
    LEARN_EXPECT_EQ(checks, copy_subrange(left_valid), (std::vector{3, 4, 5}));

    std::vector right{1, 2, 3, 4, 5};
    const auto right_valid = std::ranges::shift_right(right, 2);
    LEARN_EXPECT_EQ(checks, copy_subrange(right_valid), (std::vector{1, 2, 3}));

    std::vector unchanged{7, 8};
    const auto all = std::ranges::shift_left(unchanged, 0);
    LEARN_EXPECT_EQ(checks, copy_subrange(all), unchanged);
    const auto none = std::ranges::shift_right(unchanged, 99);
    LEARN_EXPECT(checks, none.empty());

    // Only the returned subrange has specified values; vacated elements remain valid but unspecified.
    // Complexity is at most N moves. A negative count violates the precondition.
#if 0
    const auto undefined = std::ranges::shift_left(left, -1);
#endif
#else
    std::vector modeled{1, 2, 3, 4, 5};
    const auto end = std::shift_left(modeled.begin(), modeled.end(), 2);
    LEARN_EXPECT_EQ(checks, (std::vector<int>{modeled.begin(), end}), (std::vector{3, 4, 5}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_shift >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_shift_left_right_cpp23", run>;

}  // namespace
