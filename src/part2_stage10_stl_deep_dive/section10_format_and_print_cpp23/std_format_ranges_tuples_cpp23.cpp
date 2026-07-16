// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part2/stage10/section10/std_format_ranges_tuples_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<format>)
#include <format>
#endif
#include <string>
#include <tuple>
#include <vector>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section10/std_format_ranges_tuples_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && defined(__cpp_lib_format_ranges) && \
    __cpp_lib_format_ranges >= 202207L && (!defined(LEARNCPP_HAS_FORMAT_RANGES) || LEARNCPP_HAS_FORMAT_RANGES)
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3};
    const std::tuple record{"Ada", 23};
    LEARN_EXPECT_EQ(checks, std::format("{}", values), std::string{"[1, 2, 3]"});
    LEARN_EXPECT_EQ(checks, std::format("{}", record), std::string{"(\"Ada\", 23)"});
    const std::string custom = std::format("{::02}", values);
    LEARN_EXPECT(checks, custom.contains("01"));
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "compile-and-link usable C++23 range and tuple formatting");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section10/std_format_ranges_tuples_cpp23", run>;

}  // namespace
