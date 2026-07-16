// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_left_with_iter_cpp23
// Topic id : part2/stage10/section08/ranges_fold_left_with_iter_cpp23
// Reference: WG21 P2322R6 (ranges fold algorithms).

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

#include <functional>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_fold_left_with_iter_cpp23";
    learn::ExampleChecks checks{topic};
    const std::vector values{1, 2, 3, 40};

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    const auto result = std::ranges::fold_left_with_iter(values.begin(), values.begin() + 3, 0, std::plus<>{});
    LEARN_EXPECT_EQ(checks, result.value, 6);
    LEARN_EXPECT_EQ(checks, result.in, values.begin() + 3);
    LEARN_EXPECT_EQ(checks, *result.in, 40);

    const auto first_seeded =
        std::ranges::fold_left_first_with_iter(values.begin(), values.begin() + 3, std::multiplies<>{});
    LEARN_EXPECT_EQ(checks, first_seeded.in, values.begin() + 3);
    LEARN_EXPECT(checks, first_seeded.value.has_value());
    LEARN_EXPECT_EQ(checks, *first_seeded.value, 6);

    const auto empty = std::ranges::fold_left_with_iter(values.begin(), values.begin(), 9, std::plus<>{});
    LEARN_EXPECT_EQ(checks, empty.value, 9);
    LEARN_EXPECT_EQ(checks, empty.in, values.begin());
    const auto empty_first = std::ranges::fold_left_first_with_iter(values.begin(), values.begin(), std::plus<>{});
    LEARN_EXPECT_EQ(checks, empty_first.in, values.begin());
    LEARN_EXPECT(checks, !empty_first.value.has_value());
    // Returning the terminal iterator is useful when a bounded prefix is folded. The first-seeded variant
    // represents an empty prefix with an empty optional.
#else
    int modeled = 0;
    auto iterator = values.begin();
    for (; iterator != values.begin() + 3; ++iterator) {
        modeled += *iterator;
    }
    LEARN_EXPECT_EQ(checks, modeled, 6);
    LEARN_EXPECT_EQ(checks, *iterator, 40);
    if (const int fallback_result = checks.result(); fallback_result != 0) {
        return fallback_result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_fold >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_left_with_iter_cpp23", run>;

}  // namespace
