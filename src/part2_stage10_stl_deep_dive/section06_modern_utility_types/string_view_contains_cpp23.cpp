// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_contains_cpp23
// Topic id : part2/stage10/section06/string_view_contains_cpp23

#ifdef __has_include
#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_view_contains_cpp23";

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L
[[nodiscard]] constexpr bool portable_contains(std::string_view text, std::string_view needle) noexcept {
    return text.find(needle) != std::string_view::npos;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L
    learn::ExampleChecks checks{topic_id};
    constexpr std::string_view text = "C++23 string_view";

    // The find-based spelling is the portable semantic fallback.
    static_assert(portable_contains(text, "string"));
    LEARN_EXPECT(checks, portable_contains(text, "C++23"));
    LEARN_EXPECT(checks, !portable_contains(text, "C++26"));

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    static_assert(text.contains("string"));
    static_assert(text.contains('3'));
    static_assert(text.contains(""));
    LEARN_EXPECT(checks, text.contains(std::string_view{"view"}));
    LEARN_EXPECT(checks, !text.contains('x'));
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_string_contains >= 202011L");
#endif
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::string_view");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_contains_cpp23", run>;

}  // namespace
