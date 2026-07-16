// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_thread_id_stacktrace_cpp23
// Topic id : part2/stage10/section10/std_format_thread_id_stacktrace_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<format>)
#include <format>
#endif
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#include <sstream>
#include <string>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section10/std_format_thread_id_stacktrace_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
#if defined(__cpp_lib_formatters) && __cpp_lib_formatters >= 202302L
    LEARN_EXPECT(checks, std::format("{}", std::this_thread::get_id()).size() > 0);
#else
    std::ostringstream text;
    text << std::this_thread::get_id();
    LEARN_EXPECT(checks, !text.str().empty());
#endif
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L &&                 \
    (!defined(LEARNCPP_HAS_LINKABLE_STACKTRACE) || LEARNCPP_HAS_LINKABLE_STACKTRACE) && \
    defined(__cpp_lib_formatters) && __cpp_lib_formatters >= 202302L
    const auto trace = std::stacktrace::current(0, 4);
    LEARN_EXPECT(checks, std::format("{}", trace).size() >= trace.size());
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section10/std_format_thread_id_stacktrace_cpp23", run>;

}  // namespace
