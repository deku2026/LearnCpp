// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_contains_and_subrange_cpp23
// Topic id : part2/stage10/section08/ranges_contains_and_subrange_cpp23
// Reference: WG21 P2302R4 (ranges::contains and ranges::contains_subrange).

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

#include <array>
#include <string>
#include <vector>

namespace {

struct record {
    int id;
    std::string name;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_contains_and_subrange_cpp23";
    learn::ExampleChecks checks{topic};
    const std::vector haystack{1, 2, 3, 2, 4};
    const std::array needle{2, 3};
    const std::array absent{3, 4};

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_ranges_contains) && __cpp_lib_ranges_contains >= 202207L
    LEARN_EXPECT(checks, std::ranges::contains(haystack, 4));
    LEARN_EXPECT(checks, !std::ranges::contains(haystack, 9));
    LEARN_EXPECT(checks, std::ranges::contains_subrange(haystack, needle));
    LEARN_EXPECT(checks, !std::ranges::contains_subrange(haystack, absent));
    LEARN_EXPECT(checks, std::ranges::contains_subrange(haystack, std::array<int, 0>{}));

    const std::array records{record{7, "Ada"}, record{9, "Bjarne"}};
    LEARN_EXPECT(checks, std::ranges::contains(records, 9, &record::id));

    // contains is at most N comparisons. The straightforward subrange search is O(N*M).
#else
    const auto found = std::find(haystack.begin(), haystack.end(), 4) != haystack.end();
    const auto subsequence =
        std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end()) != haystack.end();
    LEARN_EXPECT(checks, found);
    LEARN_EXPECT(checks, subsequence);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_contains >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_contains_and_subrange_cpp23", run>;

}  // namespace
