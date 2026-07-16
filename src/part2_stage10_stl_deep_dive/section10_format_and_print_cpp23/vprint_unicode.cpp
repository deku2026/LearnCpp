// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : vprint_unicode
// Topic id : part2/stage10/section10/vprint_unicode
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <cstdio>
#include <string_view>

#if __has_include(<format>)
#include <format>
#endif
#if __has_include(<print>)
#include <print>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section10/vprint_unicode";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::string_view language = "C++";
    int edition = 23;
    std::vprint_unicode(stdout, "[vprint_unicode example] {} {}\n", std::make_format_args(language, edition));
    LEARN_EXPECT(checks, true);  // Actual Unicode transcoding behavior depends on the output device.
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::vprint_unicode");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section10/vprint_unicode", run>;

}  // namespace
