// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : optional_monadic_operations_cpp23
// Topic id : part2/stage10/section06/optional_monadic_operations_cpp23

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

constexpr std::string_view topic_id = "part2/stage10/section06/optional_monadic_operations_cpp23";

#if defined(LEARN_HAS_OPTIONAL_HEADER) && defined(__cpp_lib_optional) && __cpp_lib_optional >= 201606L

[[nodiscard]] std::optional<int> parse_integer(std::string_view text) noexcept {
    int value{};
    const auto converted = std::from_chars(text.data(), text.data() + text.size(), value);
    if (converted.ec != std::errc{} || converted.ptr != text.data() + text.size()) {
        return std::nullopt;
    }
    return value;
}

[[nodiscard]] std::optional<int> keep_even(int value) noexcept {
    if (value % 2 != 0) {
        return std::nullopt;
    }
    return value;
}

[[nodiscard]] std::optional<int> manual_pipeline(std::string_view text, int fallback) noexcept {
    const auto parsed = parse_integer(text);
    if (!parsed) {
        return fallback;
    }
    const auto even = keep_even(*parsed);
    if (!even) {
        return fallback;
    }
    return *even * *even;
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_OPTIONAL_HEADER) && defined(__cpp_lib_optional) && __cpp_lib_optional >= 201606L
    learn::ExampleChecks checks{topic_id};
    LEARN_EXPECT_EQ(checks, manual_pipeline("12", -1).value_or(0), 144);
    LEARN_EXPECT_EQ(checks, manual_pipeline("11", -1).value_or(0), -1);
    LEARN_EXPECT_EQ(checks, manual_pipeline("bad", -1).value_or(0), -1);

#if __cpp_lib_optional >= 202110L
    const auto success =
        parse_integer("12").and_then(keep_even).transform([](int value) { return value * value; }).or_else([] {
            return std::optional<int>{-1};
        });
    LEARN_EXPECT_EQ(checks, success.value(), 144);

    const auto recovered =
        parse_integer("11").and_then(keep_even).transform([](int value) { return value * value; }).or_else([] {
            return std::optional<int>{-1};
        });
    LEARN_EXPECT_EQ(checks, recovered.value(), -1);

    int transform_calls{};
    const auto still_empty = std::optional<int>{}.transform([&](int value) {
        ++transform_calls;
        return value + 1;
    });
    LEARN_EXPECT(checks, !still_empty);
    LEARN_EXPECT_EQ(checks, transform_calls, 0);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_optional >= 202110L monadic API");
#endif
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::optional baseline");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/optional_monadic_operations_cpp23", run>;

}  // namespace
