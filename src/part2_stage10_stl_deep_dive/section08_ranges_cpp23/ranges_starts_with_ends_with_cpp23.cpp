// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_starts_with_ends_with_cpp23
// Topic id : part2/stage10/section08/ranges_starts_with_ends_with_cpp23
// Reference: WG21 P1659R3 (ranges::starts_with and ranges::ends_with).

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
#include <string_view>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_starts_with_ends_with_cpp23";
    learn::ExampleChecks checks{topic};
    constexpr std::string_view text = "ranges are composable";

#if LEARN_HAS_ALGORITHM_HEADER && defined(__cpp_lib_ranges_starts_ends_with) && \
    __cpp_lib_ranges_starts_ends_with >= 202106L
    LEARN_EXPECT(checks, std::ranges::starts_with(text, std::string_view{"ranges"}));
    LEARN_EXPECT(checks, std::ranges::ends_with(text, std::string_view{"composable"}));
    LEARN_EXPECT(checks, !std::ranges::starts_with(text, std::string_view{"rangez"}));
    LEARN_EXPECT(checks, std::ranges::starts_with(text, std::string_view{}));
    LEARN_EXPECT(checks, std::ranges::ends_with(text, std::string_view{}));
    LEARN_EXPECT(checks, !std::ranges::ends_with(std::array{1, 2}, std::array{0, 1, 2}));

    const std::vector signed_values{-1, -2, 3};
    const std::array magnitudes{1, 2};
    LEARN_EXPECT(checks, std::ranges::starts_with(signed_values, magnitudes, std::ranges::equal_to{},
                                                  [](int value) { return value < 0 ? -value : value; }));
    // At most min(N1,N2) comparisons after the necessary length boundary check.
#else
    const auto prefix = std::string_view{"ranges"};
    LEARN_EXPECT(checks, text.substr(0, prefix.size()) == prefix);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_starts_ends_with >= 202106L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_starts_with_ends_with_cpp23", run>;

}  // namespace
