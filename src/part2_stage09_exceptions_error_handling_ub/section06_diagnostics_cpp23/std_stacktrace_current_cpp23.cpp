// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_stacktrace_current_cpp23
// Topic id : part2/stage09/section06/std_stacktrace_current_cpp23
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#include <cstddef>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section06/std_stacktrace_current_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L && \
    (!defined(LEARNCPP_HAS_LINKABLE_STACKTRACE) || LEARNCPP_HAS_LINKABLE_STACKTRACE)
    ::learn::ExampleChecks checks{kTopic};
    const std::stacktrace trace = std::stacktrace::current(0, 8);
    LEARN_EXPECT(checks, trace.size() <= 8);
    if (!trace.empty()) {
        const std::stacktrace_entry first = trace[0];
        LEARN_EXPECT(checks, first != std::stacktrace_entry{});
    }
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "a compile-and-link capable std::stacktrace");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section06/std_stacktrace_current_cpp23", run>;

}  // namespace
