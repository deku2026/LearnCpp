// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_expected_recap_cpp23
// Topic id : part2/stage10/section06/std_expected_recap_cpp23

#ifdef __has_include
#if __has_include(<expected>)
#include <expected>
#define LEARN_HAS_EXPECTED_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <charconv>
#include <string>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_expected_recap_cpp23";

enum class MathError { invalid_integer, negative, divide_by_zero };

[[nodiscard]] constexpr std::string_view error_name(MathError error) noexcept {
    switch (error) {
        case MathError::invalid_integer:
            return "invalid integer";
        case MathError::negative:
            return "negative value";
        case MathError::divide_by_zero:
            return "divide by zero";
    }
    return "unknown";
}

struct ResultModel {
    bool has_value;
    int value;
    MathError error;
};

[[nodiscard]] ResultModel parse_model(std::string_view text) noexcept {
    int value{};
    const auto converted = std::from_chars(text.data(), text.data() + text.size(), value);
    if (converted.ec != std::errc{} || converted.ptr != text.data() + text.size()) {
        return {false, 0, MathError::invalid_integer};
    }
    if (value < 0) {
        return {false, 0, MathError::negative};
    }
    return {true, value, MathError::invalid_integer};
}

#if defined(LEARN_HAS_EXPECTED_HEADER) && defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

[[nodiscard]] std::expected<int, MathError> parse_nonnegative(std::string_view text) noexcept {
    const auto model = parse_model(text);
    if (!model.has_value) {
        return std::unexpected{model.error};
    }
    return model.value;
}

[[nodiscard]] std::expected<int, MathError> divide(int numerator, int denominator) noexcept {
    if (denominator == 0) {
        return std::unexpected{MathError::divide_by_zero};
    }
    return numerator / denominator;
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    const auto model = parse_model("84");
    LEARN_EXPECT(checks, model.has_value);
    LEARN_EXPECT_EQ(checks, model.value, 84);
    LEARN_EXPECT_EQ(checks, error_name(parse_model("oops").error), "invalid integer");

#if defined(LEARN_HAS_EXPECTED_HEADER) && defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    const auto good = parse_nonnegative("84");
    const auto bad = parse_nonnegative("-1");
    LEARN_EXPECT(checks, good.has_value());
    LEARN_EXPECT_EQ(checks, good.value(), 84);
    LEARN_EXPECT(checks, !bad.has_value());
    LEARN_EXPECT_EQ(checks, bad.error(), MathError::negative);
    LEARN_EXPECT_EQ(checks, bad.value_or(7), 7);
    LEARN_EXPECT_THROWS(checks, std::bad_expected_access<MathError>, bad.value());

    const std::expected<void, MathError> completed{};
    const std::expected<void, MathError> failed = std::unexpected{MathError::divide_by_zero};
    LEARN_EXPECT(checks, completed.has_value());
    LEARN_EXPECT_EQ(checks, failed.error(), MathError::divide_by_zero);

#if __cpp_lib_expected >= 202211L
    const auto pipeline =
        parse_nonnegative("84").and_then([](int value) { return divide(value, 2); }).transform([](int value) {
            return value + 1;
        });
    LEARN_EXPECT_EQ(checks, pipeline.value(), 43);

    const auto readable_error =
        divide(1, 0).transform_error([](MathError error) { return std::string{error_name(error)}; });
    LEARN_EXPECT(checks, !readable_error);
    LEARN_EXPECT_EQ(checks, readable_error.error(), "divide by zero");
#endif
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_expected >= 202202L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_expected_recap_cpp23", run>;

}  // namespace
