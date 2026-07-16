// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_print_println_cpp23
// Topic id : part2/stage10/section10/std_print_println_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<print>)
#include <print>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section10/std_print_println_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::print("[print example] {}", "ready");
    std::println(" code={}", 23);
    LEARN_EXPECT_EQ(checks, 20 + 3, 23);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::print/std::println");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section10/std_print_println_cpp23", run>;

}  // namespace
