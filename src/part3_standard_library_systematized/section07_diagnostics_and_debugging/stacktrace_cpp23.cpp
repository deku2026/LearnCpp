// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : stacktrace_cpp23
// Topic id : part3/section07/stacktrace_cpp23
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<stacktrace>)
#include <stacktrace>
#endif

namespace {

constexpr std::string_view kTopic = "part3/section07/stacktrace_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L && \
    (!defined(LEARNCPP_HAS_LINKABLE_STACKTRACE) || LEARNCPP_HAS_LINKABLE_STACKTRACE)
    ::learn::ExampleChecks checks{kTopic};
    const std::stacktrace trace = std::stacktrace::current(0, 8);
    LEARN_EXPECT(checks, trace.size() <= 8);
    if (!trace.empty()) {
        const auto first = trace[0];
        LEARN_EXPECT(checks, first != std::stacktrace_entry{});
    }
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "a compile-and-link capable std::stacktrace");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section07/stacktrace_cpp23", run>;

}  // namespace
