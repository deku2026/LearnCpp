// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : system_error_overview
// Topic id : part3/section07/system_error_overview
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view kTopic = "part3/section07/system_error_overview";

std::error_code open_result(bool permitted) {
    return permitted ? std::error_code{} : std::make_error_code(std::errc::permission_denied);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto success = open_result(true);
    const auto failure = open_result(false);
    LEARN_EXPECT(checks, !success);
    LEARN_EXPECT(checks, failure == std::errc::permission_denied);
    LEARN_EXPECT(checks, failure.category() == std::generic_category());
    const std::system_error exception{failure, "open"};
    LEARN_EXPECT(checks, std::string{exception.what()}.contains("open"));
    LEARN_EXPECT_EQ(checks, exception.code(), failure);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section07/system_error_overview", run>;

}  // namespace
