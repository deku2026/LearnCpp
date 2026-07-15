// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_contains_cpp23
// Topic id : part3/section01/std_string_contains_cpp23
//
// Covers: std::string::contains C++23 with feature-test fallback

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>
#include <version>

namespace {

void demo_basics() {
    std::string s = "Hello, World";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(s.contains("World"));
    assert(s.contains('H'));
    assert(!s.contains("xyz"));
#else
    assert(s.find("World") != std::string::npos);
    assert(s.find('H') != std::string::npos);
    assert(s.find("xyz") == std::string::npos);
#endif
}

void demo_intermediate() {
    std::string s = "C++23 features";
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(s.contains(std::string{"++"}));
    assert(s.contains(std::string_view{"feature"}));
#else
    assert(s.find("++") != std::string::npos);
    assert(s.find("feature") != std::string::npos);
#endif
}

void demo_expert() {
    std::string s;
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(s.contains(""));
    assert(!s.contains("a"));
#else
    assert(s.find("") != std::string::npos || s.empty());
    assert(s.find("a") == std::string::npos);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_contains_cpp23", run>;

}  // namespace
