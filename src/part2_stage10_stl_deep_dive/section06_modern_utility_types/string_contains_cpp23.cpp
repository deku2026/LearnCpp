// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_contains_cpp23
// Topic id : part2/stage10/section06/string_contains_cpp23

#ifdef __has_include
#if __has_include(<string>)
#include <string>
#define LEARN_HAS_STRING_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_contains_cpp23";

#if defined(LEARN_HAS_STRING_HEADER)
[[nodiscard]] bool portable_contains(const std::string& text, std::string_view needle) noexcept {
    return text.find(needle) != std::string::npos;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_HEADER)
    learn::ExampleChecks checks{topic_id};
    const std::string command = "compile --warnings-as-errors";
    LEARN_EXPECT(checks, portable_contains(command, "compile"));
    LEARN_EXPECT(checks, !portable_contains(command, "link"));

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_EXPECT(checks, command.contains("warnings"));
    LEARN_EXPECT(checks, command.contains('-'));
    LEARN_EXPECT(checks, command.contains(""));
    LEARN_EXPECT(checks, !command.contains('X'));
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_string_contains >= 202011L");
#endif
#else
    return learn::ExampleChecks::unavailable(topic_id, "standard <string> header");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_contains_cpp23", run>;

}  // namespace
