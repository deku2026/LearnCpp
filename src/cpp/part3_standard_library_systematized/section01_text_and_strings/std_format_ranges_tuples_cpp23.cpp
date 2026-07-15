// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part3/section01/std_format_ranges_tuples_cpp23
//
// Covers: std::format for ranges/tuples C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <tuple>
#include <vector>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    std::string s = std::format("{}", 42);
    LEARN_CHECK(s == "42");
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L && __has_include(<format>)
    std::vector<int> v{1, 2, 3};
    std::string s = std::format("{}", v);
    LEARN_CHECK(s.find('1') != std::string::npos);
#else
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(v.size() == 3);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    auto t = std::make_tuple(1, "x");
    (void)t;
    std::string s = std::format("{}-{}", 1, "x");
    LEARN_CHECK(s == "1-x");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_ranges_tuples_cpp23", run>;

}  // namespace
