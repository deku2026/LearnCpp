// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_formatter_locale_fix_cpp23
// Topic id : part2/stage10/section11/chrono_formatter_locale_fix_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <chrono>
#include <string_view>
#if __has_include(<format>)
#include <format>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/chrono_formatter_locale_fix_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    ::learn::ExampleChecks checks{kTopic};
    using namespace std::chrono;
    const sys_days day = year{2026} / July / 16;
    const std::string text = std::format("{:%F}", day);
    LEARN_EXPECT_EQ(checks, text, std::string{"2026-07-16"});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "chrono formatting");
#endif
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section11/chrono_formatter_locale_fix_cpp23", run>;

}  // namespace
