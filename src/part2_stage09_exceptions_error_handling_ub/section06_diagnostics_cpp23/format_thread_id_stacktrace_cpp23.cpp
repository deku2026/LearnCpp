// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : format_thread_id_stacktrace_cpp23
// Topic id : part2/stage09/section06/format_thread_id_stacktrace_cpp23
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<format>)
#include <format>
#endif
#include <sstream>
#include <string>
#include <thread>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section06/format_thread_id_stacktrace_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
#if defined(__cpp_lib_formatters) && __cpp_lib_formatters >= 202302L
    const std::string formatted = std::format("worker={}", std::this_thread::get_id());
    LEARN_EXPECT(checks, formatted.starts_with("worker="));
#else
    std::ostringstream output;
    output << "worker=" << std::this_thread::get_id();
    LEARN_EXPECT(checks, output.str().starts_with("worker="));
#endif
    // Stacktrace formatting has a separate availability axis; its dedicated topic probes it.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage09/section06/format_thread_id_stacktrace_cpp23", run>;

}  // namespace
