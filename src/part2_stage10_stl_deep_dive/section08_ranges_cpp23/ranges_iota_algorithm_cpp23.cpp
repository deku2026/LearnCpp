// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_iota_algorithm_cpp23
// Topic id : part2/stage10/section08/ranges_iota_algorithm_cpp23
// Reference: WG21 P2440R1 (ranges::iota and ranges shift algorithms).

#include "learn/example_support.hpp"

#if __has_include(<numeric>)
#include <numeric>
#define LEARN_HAS_NUMERIC_HEADER 1
#else
#define LEARN_HAS_NUMERIC_HEADER 0
#endif

#include <algorithm>
#include <array>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_iota_algorithm_cpp23";
    learn::ExampleChecks checks{topic};
    std::array<int, 5> values{};

#if LEARN_HAS_NUMERIC_HEADER && defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
    const auto result = std::ranges::iota(values, 10);
    LEARN_EXPECT_EQ(checks, values, (std::array{10, 11, 12, 13, 14}));
    LEARN_EXPECT_EQ(checks, result.out, values.end());
    LEARN_EXPECT_EQ(checks, result.value, 15);

    std::array<int, 0> empty{};
    const auto empty_result = std::ranges::iota(empty, 7);
    LEARN_EXPECT_EQ(checks, empty_result.value, 7);
    // Eager algorithm: exactly distance(output) assignments and increments.
#else
    std::iota(values.begin(), values.end(), 10);
    LEARN_EXPECT_EQ(checks, values, (std::array{10, 11, 12, 13, 14}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_iota >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_iota_algorithm_cpp23", run>;

}  // namespace
