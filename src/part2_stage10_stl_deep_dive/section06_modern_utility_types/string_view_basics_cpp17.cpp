// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_basics_cpp17
// Topic id : part2/stage10/section06/string_view_basics_cpp17

#ifdef __has_include
#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <array>
#include <optional>
#include <string>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_view_basics_cpp17";

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L

struct HeaderView {
    std::string_view name;
    std::string_view value;
};

[[nodiscard]] constexpr std::string_view trim(std::string_view text) noexcept {
    constexpr std::string_view whitespace = " \t\r\n";
    const auto first = text.find_first_not_of(whitespace);
    if (first == std::string_view::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1U);
}

[[nodiscard]] constexpr std::optional<HeaderView> split_header(std::string_view line) noexcept {
    const auto colon = line.find(':');
    if (colon == std::string_view::npos) {
        return std::nullopt;
    }
    return HeaderView{trim(line.substr(0, colon)), trim(line.substr(colon + 1U))};
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L
    learn::ExampleChecks checks{topic_id};

    // string_view owns neither storage nor a terminator; the std::string remains alive here.
    const std::string line = "  Content-Type : text/plain  ";
    const auto header = split_header(line);
    LEARN_EXPECT(checks, header.has_value());
    LEARN_EXPECT_EQ(checks, header->name, "Content-Type");
    LEARN_EXPECT_EQ(checks, header->value, "text/plain");
    LEARN_EXPECT(checks, header->name.data() >= line.data());
    LEARN_EXPECT(checks, header->value.data() < line.data() + line.size());

    // A view can describe a non-null-terminated slice, so pass size together with data().
    const std::array raw{'a', 'l', 'p', 'h', 'a'};
    const std::string_view middle{raw.data() + 1, 3};
    LEARN_EXPECT_EQ(checks, middle, "lph");
    LEARN_EXPECT_EQ(checks, middle.size(), 3U);

    std::string_view route = "/api/v1/users?id=42";
    route.remove_prefix(std::string_view{"/api/"}.size());
    const auto query = route.find('?');
    LEARN_EXPECT_EQ(checks, route.substr(0, query), "v1/users");
    LEARN_EXPECT_EQ(checks, route.substr(query + 1U), "id=42");

    constexpr auto compile_time = trim("  zero-copy  ");
    static_assert(compile_time == "zero-copy");
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::string_view");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_basics_cpp17", run>;

}  // namespace
