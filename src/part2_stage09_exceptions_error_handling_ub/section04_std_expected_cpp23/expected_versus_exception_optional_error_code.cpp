// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_versus_exception_optional_error_code
// Topic id : part2/stage09/section04/expected_versus_exception_optional_error_code
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/expected_versus_exception_optional_error_code";

std::optional<int> optional_lookup(bool found) {
    return found ? std::optional{4} : std::nullopt;
}

int exception_lookup(bool found) {
    if (!found) {
        throw std::runtime_error{"missing"};
    }
    return 4;
}

std::error_code error_code_lookup(bool found, int& output) {
    if (!found) {
        return std::make_error_code(std::errc::no_such_file_or_directory);
    }
    output = 4;
    return {};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, !optional_lookup(false));  // Absence only; no reason is carried.
    LEARN_EXPECT_THROWS(checks, std::runtime_error, exception_lookup(false));
    int output{};
    LEARN_EXPECT(checks, static_cast<bool>(error_code_lookup(false, output)));
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    const std::expected<int, std::string> explicit_failure{std::unexpected{"missing"}};
    LEARN_EXPECT_EQ(checks, explicit_failure.error(), std::string{"missing"});
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage09/section04/expected_versus_exception_optional_error_code", run>;

}  // namespace
