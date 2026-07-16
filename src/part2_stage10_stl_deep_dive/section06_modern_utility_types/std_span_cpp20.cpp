// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_span_cpp20
// Topic id : part2/stage10/section06/std_span_cpp20

#ifdef __has_include
#if __has_include(<span>)
#include <span>
#define LEARN_HAS_SPAN_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <numeric>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_span_cpp20";

#if defined(LEARN_HAS_SPAN_HEADER) && defined(__cpp_lib_span) && __cpp_lib_span >= 202002L

[[nodiscard]] int sum(std::span<const int> values) {
    return std::accumulate(values.begin(), values.end(), 0);
}

void scale(std::span<int> values, int factor) {
    for (int& value : values) {
        value *= factor;
    }
}

[[nodiscard]] constexpr int dot(std::span<const int, 3> left, std::span<const int, 3> right) noexcept {
    return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_SPAN_HEADER) && defined(__cpp_lib_span) && __cpp_lib_span >= 202002L
    learn::ExampleChecks checks{topic_id};

    std::vector values{1, 2, 3, 4, 5};
    const std::span all{values};  // dynamic extent, non-owning and contiguous.
    LEARN_EXPECT_EQ(checks, sum(all), 15);

    scale(all.subspan(1, 3), 10);
    LEARN_EXPECT_EQ(checks, (values), (std::vector{1, 20, 30, 40, 5}));
    LEARN_EXPECT_EQ(checks, all.data(), values.data());

    constexpr std::array x{1, 2, 3};
    constexpr std::array y{4, 5, 6};
    static_assert(dot(std::span<const int, 3>{x}, std::span<const int, 3>{y}) == 32);

    const auto bytes = std::as_bytes(all);
    LEARN_EXPECT_EQ(checks, bytes.size(), all.size_bytes());
    static_assert(decltype(std::span{x})::extent == 3);
    static_assert(std::span<int>::extent == std::dynamic_extent);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++20 __cpp_lib_span >= 202002L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_span_cpp20", run>;

}  // namespace
