// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_join_with_cpp23
// Topic id : part2/stage10/section08/views_join_with_cpp23
// Reference: WG21 P2441R2 (views::join_with).

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
#include <string_view>
#include <vector>

namespace {

std::string collect(auto&& characters) {
    return {characters.begin(), characters.end()};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_join_with_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector<std::string> words{"ranges", "are", "lazy"};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_join_with) && __cpp_lib_ranges_join_with >= 202202L
    auto comma_joined = words | std::views::join_with(',');
    LEARN_EXPECT_EQ(checks, collect(comma_joined), std::string{"ranges,are,lazy"});

    constexpr std::string_view delimiter{" / "};
    auto decorated = words | std::views::join_with(delimiter);
    LEARN_EXPECT_EQ(checks, collect(decorated), std::string{"ranges / are / lazy"});

    words[1] = "stay";
    LEARN_EXPECT_EQ(checks, collect(comma_joined), std::string{"ranges,stay,lazy"});
    LEARN_EXPECT(checks, collect(std::vector<std::string>{} | std::views::join_with(',')).empty());
    // Delimiters occur only between inner ranges. Keep referenced outer/inner storage alive.
#else
    std::string modeled;
    for (const auto& word : words) {
        if (!modeled.empty()) {
            modeled += ',';
        }
        modeled += word;
    }
    LEARN_EXPECT_EQ(checks, modeled, std::string{"ranges,are,lazy"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_join_with >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_join_with_cpp23", run>;

}  // namespace
