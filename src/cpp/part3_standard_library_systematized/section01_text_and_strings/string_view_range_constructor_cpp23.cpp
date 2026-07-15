// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_range_constructor_cpp23
// Topic id : part3/section01/string_view_range_constructor_cpp23
//
// Covers: string_view construction from contiguous ranges C++23

#include "learn/topic_registry.hpp"

#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    std::string s = "from-string";
    std::string_view sv{s};
    LEARN_CHECK(sv == "from-string");
}

void demo_intermediate() {
    std::array<char, 4> a{{'a', 'b', 'c', 'd'}};
    std::string_view sv{a.data(), a.size()};
    LEARN_CHECK(sv == "abcd");
}

void demo_expert() {
#if defined(__cpp_lib_ranges_to_container) || defined(__cpp_lib_string_view)
    std::vector<char> v{'x', 'y', 'z'};
    std::string_view sv{v.data(), v.size()};
    LEARN_CHECK(sv == "xyz");
#else
    const char buf[] = "xyz";
    std::string_view sv{buf, 3};
    LEARN_CHECK(sv == "xyz");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_range_constructor_cpp23", run>;

}  // namespace
