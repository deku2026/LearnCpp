// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_repeat_cpp23
// Topic id : part2/stage10/section08/views_repeat_cpp23
// Reference: WG21 P2474R2 (views::repeat).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<ranges>)
#include <ranges>
#define LEARN_HAS_RANGES_HEADER 1
#else
#define LEARN_HAS_RANGES_HEADER 0
#endif

#include <string>
#include <vector>

namespace {

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_repeat) && __cpp_lib_ranges_repeat >= 202207L
template <std::ranges::input_range Range>
std::vector<std::ranges::range_value_t<Range>> collect(Range&& range) {
    return {range.begin(), range.end()};
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_repeat_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_repeat) && __cpp_lib_ranges_repeat >= 202207L
    std::string seed{"echo"};
    auto bounded = std::views::repeat(seed, 3);
    seed = "changed";
    LEARN_EXPECT_EQ(checks, collect(bounded), (std::vector<std::string>{"echo", "echo", "echo"}));

    auto capped_infinite = std::views::repeat(7) | std::views::take(4);
    LEARN_EXPECT_EQ(checks, collect(capped_infinite), (std::vector{7, 7, 7, 7}));
    LEARN_EXPECT(checks, std::views::repeat(1, 0).empty());
    // Unbounded repeat is infinite: every real traversal must impose a terminating boundary.
#if 0
    for (int value : std::views::repeat(7)) { /* never terminates */ }
#endif
#else
    LEARN_EXPECT_EQ(checks, (std::vector<std::string>(3, "echo")), (std::vector<std::string>{"echo", "echo", "echo"}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_repeat >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_repeat_cpp23", run>;

}  // namespace
