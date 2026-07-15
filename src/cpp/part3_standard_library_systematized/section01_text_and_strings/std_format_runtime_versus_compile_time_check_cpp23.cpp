// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_runtime_versus_compile_time_check_cpp23
// Topic id : part3/section01/std_format_runtime_versus_compile_time_check_cpp23
//
// Covers: compile-time format string checks vs runtime format_string

#include "learn/topic_registry.hpp"

#include <cassert>
#include <format>
#include <string>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // literal format strings are checked at compile time
    std::string s = std::format("value={}", 10);
    assert(s == "value=10");
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const int n = 3;
    std::string s = std::format("n={:d}", n);
    assert(s == "n=3");
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    std::string fmt = "{}";
    int value = 99;
    std::string s = std::vformat(fmt, std::make_format_args(value));
    assert(s == "99");
#else
    assert(true);
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
    ::learn::topic<"part3/section01/std_format_runtime_versus_compile_time_check_cpp23", run>;

}  // namespace
