// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_compile_time_check_cpp23
// Topic id : part2/stage10/section10/std_format_compile_time_check_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<format>)
#include <format>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section10/std_format_compile_time_check_cpp23";

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
template <class... Arguments>
std::string checked_format(std::format_string<Arguments...> pattern, Arguments&&... arguments) {
    return std::format(pattern, std::forward<Arguments>(arguments)...);
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    ::learn::ExampleChecks checks{kTopic};
    const std::string text = checked_format("value={:04}", 23);
    LEARN_EXPECT_EQ(checks, text, std::string{"value=0023"});
    const std::string runtime_pattern = "{} + {} = {}";
    int left = 2;
    int right = 3;
    int total = 5;
    LEARN_EXPECT_EQ(checks, std::vformat(runtime_pattern, std::make_format_args(left, right, total)),
                    std::string{"2 + 3 = 5"});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::format");
#endif
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section10/std_format_compile_time_check_cpp23", run>;

}  // namespace
