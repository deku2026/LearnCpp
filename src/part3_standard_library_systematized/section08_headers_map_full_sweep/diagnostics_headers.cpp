// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : diagnostics_headers
// Topic id : part3/section08/diagnostics_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <exception>
#include <source_location>
#include <stdexcept>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view kTopic = "part3/section08/diagnostics_headers";

constexpr std::array<std::string_view, 10> headers{
    "cassert",   "cerrno",       "exception", "source_location", "stacktrace",
    "stdexcept", "system_error", "debugging", "contracts",       "version",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto location = std::source_location::current();
    LEARN_EXPECT(checks, std::string_view{location.function_name()}.contains("run"));
    LEARN_EXPECT_THROWS(checks, std::runtime_error, throw std::runtime_error{"diagnostic"});
    const auto code = std::make_error_code(std::errc::invalid_argument);
    LEARN_EXPECT(checks, static_cast<bool>(code));
    LEARN_EXPECT_EQ(checks, headers.size(), 10U);
    // Later-standard headers such as <debugging> and <contracts> remain map entries, not assumed capabilities.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/diagnostics_headers", run>;

}  // namespace
