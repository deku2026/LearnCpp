// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_find_last_family_cpp23
// Topic id : part2/stage10/section08/ranges_find_last_family_cpp23
// Reference: WG21 P1223R5 (find_last, find_last_if, find_last_if_not).

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

#include <string>
#include <vector>

namespace {

struct item {
    int key;
    std::string label;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_find_last_family_cpp23";
    learn::ExampleChecks checks{topic};
    const std::vector values{1, 4, 3, 4, 6};

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
    const auto last_four = std::ranges::find_last(values, 4);
    LEARN_EXPECT(checks, last_four.begin() != last_four.end());
    LEARN_EXPECT_EQ(checks, *last_four.begin(), 4);
    LEARN_EXPECT_EQ(checks, std::ranges::distance(last_four), 2);

    const auto last_odd = std::ranges::find_last_if(values, [](int value) { return value % 2 != 0; });
    const auto last_not_even = std::ranges::find_last_if_not(values, [](int value) { return value % 2 == 0; });
    LEARN_EXPECT_EQ(checks, *last_odd.begin(), 3);
    LEARN_EXPECT_EQ(checks, last_odd.begin(), last_not_even.begin());

    const std::vector items{item{1, "first"}, item{2, "middle"}, item{1, "last"}};
    const auto projected = std::ranges::find_last(items, 1, &item::key);
    LEARN_EXPECT_EQ(checks, projected.begin()->label, std::string{"last"});

    const auto missing = std::ranges::find_last(values, 99);
    LEARN_EXPECT(checks, missing.empty());
    // The result is [last match, original end), not merely an iterator. Complexity is O(N).
#else
    const auto reverse = std::find(values.rbegin(), values.rend(), 4);
    LEARN_EXPECT(checks, reverse != values.rend());
    LEARN_EXPECT_EQ(checks, *reverse, 4);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_find_last >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_find_last_family_cpp23", run>;

}  // namespace
