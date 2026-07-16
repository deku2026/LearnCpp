// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_right_last_cpp23
// Topic id : part2/stage10/section08/ranges_fold_right_last_cpp23
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
    constexpr auto topic = "part2/stage10/section08/ranges_fold_right_last_cpp23";
    learn::ExampleChecks checks{topic};
    const std::vector values{8, 4, 2};

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    const auto quotient = std::ranges::fold_right_last(values, std::divides<>{});
    LEARN_EXPECT(checks, quotient.has_value());
    LEARN_EXPECT_EQ(checks, *quotient, 4);  // 8 / (4 / 2)

    const auto singleton = std::ranges::fold_right_last(std::vector{11}, std::plus<>{});
    LEARN_EXPECT_EQ(checks, *singleton, 11);
    const auto empty = std::ranges::fold_right_last(std::vector<int>{}, std::plus<>{});
    LEARN_EXPECT(checks, !empty.has_value());
    // The last element becomes the implicit seed; an empty input produces no value.
#else
    int modeled = values.back();
    for (auto iterator = values.rbegin() + 1; iterator != values.rend(); ++iterator) {
        modeled = *iterator / modeled;
    }
    LEARN_EXPECT_EQ(checks, modeled, 4);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_fold >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_right_last_cpp23", run>;

}  // namespace
