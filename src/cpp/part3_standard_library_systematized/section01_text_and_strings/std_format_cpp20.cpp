// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_cpp20
// Topic id : part3/section01/std_format_cpp20
//
// Covers: std::format basic formatting C++20

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    std::string s = std::format("{} + {} = {}", 1, 2, 3);
    LEARN_CHECK(s == "1 + 2 = 3");
#else
    std::string s = "1 + 2 = 3";
    LEARN_CHECK(s.size() == 9);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    LEARN_CHECK(std::format("{:04}", 7) == "0007");
    LEARN_CHECK(std::format("{:.2f}", 3.14159) == "3.14");
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    LEARN_CHECK(std::format("{:>5}", 42) == "   42");
    LEARN_CHECK(std::format("{:x}", 255) == "ff");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_cpp20", run>;

}  // namespace
