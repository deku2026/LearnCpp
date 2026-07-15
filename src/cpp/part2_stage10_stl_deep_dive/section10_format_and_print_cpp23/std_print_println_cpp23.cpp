// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_print_println_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_print_println_cpp23
//
// Covers: std::print / println C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <version>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#endif
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const auto s = std::format("Hello {}", 42);
    assert(s == "Hello 42");
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    // print to stringstream via format for assertable demo
    const auto s = std::format("x={} y={}", 1, 2);
    assert(s == "x=1 y=2");
#else
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    assert(std::format("{}", 7) == "7");
#else
    assert(true);
#endif
#endif
}

void demo_expert() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    // Prefer print/println over iostream for formatted output when available
    // Avoid calling print in unit demos that capture stdout; use format
    assert(std::format("{:.2f}", 3.14159) == "3.14");
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    assert(std::format("{:.2f}", 3.14159) == "3.14");
#else
    assert(true);
#endif
}

}  // namespace

namespace {

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
