// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_cpp20
// Topic id : part3/section01/std_format_cpp20
//
// Covers: std::format basic formatting C++20

#include "learn/topic_registry.hpp"

#include <cassert>
#include <format>
#include <string>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    std::string s = std::format("{} + {} = {}", 1, 2, 3);
    assert(s == "1 + 2 = 3");
#else
    std::string s = "1 + 2 = 3";
    assert(s.size() == 9);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    assert(std::format("{:04}", 7) == "0007");
    assert(std::format("{:.2f}", 3.14159) == "3.14");
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    assert(std::format("{:>5}", 42) == "   42");
    assert(std::format("{:x}", 255) == "ff");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_cpp20", run>;

}  // namespace
