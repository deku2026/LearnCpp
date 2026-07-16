// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_to_container_cpp23
// Topic id : part2/stage10/section08/ranges_to_container_cpp23
// Reference: WG21 P1206R7 (ranges::to).

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

#include <list>
#include <set>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_to_container_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
    std::vector source{1, 2, 2, 3, 4};
    auto even_squares = source | std::views::filter([](int value) { return value % 2 == 0; }) |
                        std::views::transform([](int value) { return value * value; });

    const auto vector_result = even_squares | std::ranges::to<std::vector>();
    const auto set_result = even_squares | std::ranges::to<std::set>();
    const auto list_result = std::ranges::to<std::list<int>>(even_squares);
    LEARN_EXPECT_EQ(checks, vector_result, (std::vector{4, 4, 16}));
    LEARN_EXPECT_EQ(checks, set_result, (std::set{4, 16}));
    LEARN_EXPECT_EQ(checks, (std::vector<int>{list_result.begin(), list_result.end()}), vector_result);

    source[1] = 100;
    LEARN_EXPECT_EQ(checks, vector_result, (std::vector{4, 4, 16}));
    LEARN_EXPECT_EQ(checks, *even_squares.begin(), 10'000);
    // to materializes an owning snapshot: O(N) insertions, plus container-specific costs.
#else
    const std::vector source{1, 2, 2, 3, 4};
    std::vector<int> modeled;
    for (int value : source) {
        if (value % 2 == 0) {
            modeled.push_back(value * value);
        }
    }
    LEARN_EXPECT_EQ(checks, modeled, (std::vector{4, 4, 16}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_to_container >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_to_container_cpp23", run>;

}  // namespace
