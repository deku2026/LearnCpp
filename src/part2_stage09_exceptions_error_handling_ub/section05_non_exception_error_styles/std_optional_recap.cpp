// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_optional_recap
// Topic id : part2/stage09/section05/std_optional_recap
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <optional>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section05/std_optional_recap";

std::optional<int> find_port(std::string_view service) {
    if (service == "https") {
        return 443;
    }
    if (service == "http") {
        return 80;
    }
    return std::nullopt;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto secure = find_port("https");
    const auto absent = find_port("smtp");
    LEARN_EXPECT_EQ(checks, secure.value(), 443);
    LEARN_EXPECT(checks, !absent.has_value());
    LEARN_EXPECT_EQ(checks, absent.value_or(25), 25);
    LEARN_EXPECT_THROWS(checks, std::bad_optional_access, absent.value());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section05/std_optional_recap", run>;

}  // namespace
