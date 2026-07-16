// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : ranges_fold_overview_cpp23
// Topic id : part3/section02/ranges_fold_overview_cpp23
// Reference: WG21 P2322R6 (__cpp_lib_ranges_fold).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <algorithm>
#include <functional>
#include <numeric>
#include <optional>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/ranges_fold_overview_cpp23";
    learn::ExampleChecks checks{topic};
    const std::vector values{1, 2, 3, 4};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    LEARN_EXPECT_EQ(checks, std::ranges::fold_left(values, 0, std::plus<>{}), 10);
    LEARN_EXPECT_EQ(checks, std::ranges::fold_left(values, 100, std::minus<>{}), 90);
    LEARN_EXPECT_EQ(checks, std::ranges::fold_right(values, 100, std::minus<>{}), 98);

    const auto first = std::ranges::fold_left_first(values, std::plus<>{});
    const auto empty_first = std::ranges::fold_left_first(std::vector<int>{}, std::plus<>{});
    const auto last = std::ranges::fold_right_last(values, std::minus<>{});
    LEARN_EXPECT(checks, first.has_value());
    LEARN_EXPECT_EQ(checks, *first, 10);
    LEARN_EXPECT(checks, !empty_first.has_value());
    LEARN_EXPECT_EQ(checks, *last, -2);

    const auto with_position =
        std::ranges::fold_left_with_iter(values, 0, [](int sum, int value) { return sum + value; });
    LEARN_EXPECT(checks, with_position.in == values.end());
    LEARN_EXPECT_EQ(checks, with_position.value, 10);

    // fold_left groups (((init op e1) op e2) ...); fold_right groups in the opposite direction.
    // The *_first/*_last forms obtain the initial value from the range and return optional for an
    // empty range. Unlike reduce, fold order is specified, so non-associative operations are valid.
    return checks.result();
#else
    LEARN_EXPECT_EQ(checks, std::accumulate(values.begin(), values.end(), 0), 10);
    int modeled_right = 100;
    for (auto cursor = values.rbegin(); cursor != values.rend(); ++cursor) {
        modeled_right = *cursor - modeled_right;
    }
    LEARN_EXPECT_EQ(checks, modeled_right, 98);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_fold >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/ranges_fold_overview_cpp23", run>;

}  // namespace
