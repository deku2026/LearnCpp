// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : non_const_formattable_cpp23
// Topic id : part2/stage10/section10/non_const_formattable_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<format>)
#include <format>
#endif
#include <string>
#include <version>

namespace non_const_formattable_demo {

struct Cursor {
    int value{};
    int read() & { return value; }
    int read() const& = delete;
};

}  // namespace non_const_formattable_demo

#if defined(__cpp_lib_format) && __cpp_lib_format >= 202207L
template <>
struct std::formatter<non_const_formattable_demo::Cursor> {
    constexpr auto parse(std::format_parse_context& context) { return context.begin(); }

    auto format(non_const_formattable_demo::Cursor& cursor, std::format_context& context) const {
        return std::format_to(context.out(), "cursor={}", cursor.read());
    }
};
#endif

namespace {

constexpr std::string_view kTopic = "part2/stage10/section10/non_const_formattable_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_format) && __cpp_lib_format >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    non_const_formattable_demo::Cursor cursor{7};
    LEARN_EXPECT_EQ(checks, std::format("{}", cursor), std::string{"cursor=7"});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "C++23 non-const formattable arguments");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section10/non_const_formattable_cpp23", run>;

}  // namespace
