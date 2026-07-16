// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_print_for_diagnostics_cpp23
// Topic id : part2/stage09/section06/std_print_for_diagnostics_cpp23
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>
#if __has_include(<print>)
#include <print>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section06/std_print_for_diagnostics_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::print("[example diagnostic] code={} state={}\n", 7, "ready");
    LEARN_EXPECT_EQ(checks, 3 + 4, 7);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::print");
#endif
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage09/section06/std_print_for_diagnostics_cpp23", run>;

}  // namespace
