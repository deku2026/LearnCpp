// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : try_catch_throw_basics
// Topic id : part2/stage09/section01/try_catch_throw_basics
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section01/try_catch_throw_basics";

int parse_age(std::string_view text) {
    if (text.empty()) {
        throw std::invalid_argument{"age is empty"};
    }
    int value{};
    for (const char ch : text) {
        if (ch < '0' || ch > '9') {
            throw std::invalid_argument{"age is not decimal"};
        }
        value = value * 10 + (ch - '0');
    }
    if (value > 150) {
        throw std::out_of_range{"age is outside the model"};
    }
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, parse_age("42"), 42);
    LEARN_EXPECT_THROWS(checks, std::invalid_argument, parse_age("4x"));
    LEARN_EXPECT_THROWS(checks, std::out_of_range, parse_age("151"));

    bool caught_by_reference = false;
    try {
        static_cast<void>(parse_age(""));
    } catch (const std::invalid_argument& error) {
        caught_by_reference = std::string_view{error.what()}.starts_with("age");
    }
    LEARN_EXPECT(checks, caught_by_reference);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section01/try_catch_throw_basics", run>;

}  // namespace
