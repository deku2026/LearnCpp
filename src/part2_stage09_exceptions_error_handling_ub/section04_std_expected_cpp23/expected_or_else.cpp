// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_or_else
// Topic id : part2/stage09/section04/expected_or_else
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/expected_or_else";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
std::expected<int, std::string> recover_missing(const std::string& error) {
    if (error == "missing") {
        return 10;
    }
    return std::unexpected{error};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::expected<int, std::string> missing{std::unexpected{"missing"}};
    const std::expected<int, std::string> corrupt{std::unexpected{"corrupt"}};
    LEARN_EXPECT_EQ(checks, missing.or_else(recover_missing).value(), 10);
    LEARN_EXPECT_EQ(checks, corrupt.or_else(recover_missing).error(), std::string{"corrupt"});
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "expected::or_else");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/expected_or_else", run>;

}  // namespace
