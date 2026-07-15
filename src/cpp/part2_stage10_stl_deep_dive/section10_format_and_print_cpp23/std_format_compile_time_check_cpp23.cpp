// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_compile_time_check_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_format_compile_time_check_cpp23
//
// Covers: format compile-time format string checking C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // Format string is checked against argument types (consteval in C++20/23)
    const auto s = std::format("{} {}", 1, "two");
    LEARN_CHECK(s == "1 two");
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const auto s = std::format("{:04d}", 42);
    LEARN_CHECK(s == "0042");
    const auto h = std::format("{:x}", 255);
    LEARN_CHECK(h == "ff");
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // compile-time checked format string; prefer std::format over vformat/make_format_args
    const auto s = std::format("{}-{}", 1, 2);
    LEARN_CHECK(s == "1-2");
#else
    LEARN_CHECK(true);
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
    ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/std_format_compile_time_check_cpp23", run>;

}  // namespace
