// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_right_cpp23
// Topic id : part2/stage10/section08/ranges_fold_right_cpp23
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
#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_fold_right_cpp23";
    learn::ExampleChecks checks{topic};
    const std::vector values{1, 2, 3};

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    LEARN_EXPECT_EQ(checks, std::ranges::fold_right(values, 0, std::minus<>{}), 2);  // 1-(2-(3-0))
    const auto trace = std::ranges::fold_right(values, std::string{"z"}, [](int value, std::string acc) {
        return "(" + std::to_string(value) + "+" + std::move(acc) + ")";
    });
    LEARN_EXPECT_EQ(checks, trace, std::string{"(1+(2+(3+z)))"});
    LEARN_EXPECT_EQ(checks, std::ranges::fold_right(std::vector<int>{}, 7, std::plus<>{}), 7);

    static_assert(std::ranges::bidirectional_range<decltype(values)>);
    // fold_right needs bidirectional traversal and invokes the operation exactly N times.
#else
    int modeled = 0;
    for (auto iterator = values.rbegin(); iterator != values.rend(); ++iterator) {
        modeled = *iterator - modeled;
    }
    LEARN_EXPECT_EQ(checks, modeled, 2);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_fold >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_right_cpp23", run>;

}  // namespace
