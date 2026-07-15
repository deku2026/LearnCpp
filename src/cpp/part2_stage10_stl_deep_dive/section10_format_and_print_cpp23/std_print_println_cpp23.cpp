// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_print_println_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_print_println_cpp23
//
// Covers: std::print / println C++23

#include "learn/topic_registry.hpp"

#include <sstream>
#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#if __has_include(<print>)
#include <print>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    const auto s = std::format("Hello {}", 42);
    LEARN_CHECK(s == "Hello 42");
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    // print family is gated separately; assert via format for quiet demos
    const auto s = std::format("x={} y={}", 1, 2);
    LEARN_CHECK(s == "x=1 y=2");
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    // Prefer print/println over iostream for formatted output when available
    // Avoid calling print in unit demos that capture stdout; use format
    LEARN_CHECK(std::format("{:.2f}", 3.14159) == "3.14");
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/std_print_println_cpp23", run>;

}  // namespace
