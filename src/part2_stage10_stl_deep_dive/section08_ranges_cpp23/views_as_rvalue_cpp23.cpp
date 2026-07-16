// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_as_rvalue_cpp23
// Topic id : part2/stage10/section08/views_as_rvalue_cpp23
// Reference: WG21 P2446R2 (views::as_rvalue).

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

#include <concepts>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_as_rvalue_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector<std::string> source{"alpha", "beta"};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_as_rvalue) && __cpp_lib_ranges_as_rvalue >= 202207L
    auto movable = source | std::views::as_rvalue;
    static_assert(std::same_as<std::ranges::range_reference_t<decltype(movable)>, std::string&&>);
    source[0] = "updated";  // The view has not eagerly read or moved anything.

    std::vector<std::string> destination;
    for (std::string&& value : movable) {
        destination.push_back(std::move(value));
    }
    LEARN_EXPECT_EQ(checks, destination, (std::vector<std::string>{"updated", "beta"}));
    LEARN_EXPECT_EQ(checks, source.size(), std::size_t{2});
    // Each moved-from string is valid but unspecified: never assert that it is empty.
#else
    std::vector<std::string> destination;
    for (auto& value : source) {
        destination.push_back(std::move(value));
    }
    LEARN_EXPECT_EQ(checks, destination, (std::vector<std::string>{"alpha", "beta"}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_as_rvalue >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_as_rvalue_cpp23", run>;

}  // namespace
