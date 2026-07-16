// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_optional
// Topic id : part2/stage10/section06/std_optional

#ifdef __has_include
#if __has_include(<optional>)
#include <optional>
#define LEARN_HAS_OPTIONAL_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <charconv>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_optional";

struct ParseModel {
    bool has_value;
    int value;
};

[[nodiscard]] ParseModel parse_model(std::string_view text) noexcept {
    int value{};
    const auto converted = std::from_chars(text.data(), text.data() + text.size(), value);
    return {converted.ec == std::errc{} && converted.ptr == text.data() + text.size(), value};
}

#if defined(LEARN_HAS_OPTIONAL_HEADER) && defined(__cpp_lib_optional) && __cpp_lib_optional >= 201606L
[[nodiscard]] std::optional<int> parse_integer(std::string_view text) noexcept {
    const auto model = parse_model(text);
    if (!model.has_value) {
        return std::nullopt;
    }
    return model.value;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    const auto model = parse_model("123");
    LEARN_EXPECT(checks, model.has_value);
    LEARN_EXPECT_EQ(checks, model.value, 123);
    LEARN_EXPECT(checks, !parse_model("12x").has_value);

#if defined(LEARN_HAS_OPTIONAL_HEADER) && defined(__cpp_lib_optional) && __cpp_lib_optional >= 201606L
    auto number = parse_integer("42");
    LEARN_EXPECT(checks, number.has_value());
    LEARN_EXPECT_EQ(checks, *number, 42);
    LEARN_EXPECT_EQ(checks, parse_integer("4x").value_or(-1), -1);
    LEARN_EXPECT_EQ(checks, parse_integer("").value_or(-1), -1);

    number.reset();
    LEARN_EXPECT(checks, !number);
    LEARN_EXPECT_THROWS(checks, std::bad_optional_access, number.value());
    int& stored = number.emplace(7);
    stored *= 6;
    LEARN_EXPECT_EQ(checks, number.value(), 42);

    constexpr std::optional<int> answer{42};
    static_assert(answer.has_value() && answer.value() == 42);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++17 __cpp_lib_optional >= 201606L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_optional", run>;

}  // namespace
