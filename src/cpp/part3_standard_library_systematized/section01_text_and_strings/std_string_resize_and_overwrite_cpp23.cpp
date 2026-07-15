// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_resize_and_overwrite_cpp23
// Topic id : part3/section01/std_string_resize_and_overwrite_cpp23
//
// Covers: std::string::resize_and_overwrite C++23 high-performance fill

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string>
#include <version>

namespace {

void demo_basics() {
    std::string s = "abcd";
    s.resize(2);
    LEARN_CHECK(s == "ab");
    s.resize(5, 'x');
    LEARN_CHECK(s == "abxxx");
}

void demo_intermediate() {
#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string buf;
    buf.resize_and_overwrite(5, [](char* p, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            p[i] = static_cast<char>('A' + static_cast<int>(i));
        }
        return n;
    });
    LEARN_CHECK(buf == "ABCDE");
#else
    std::string buf = "ABCDE";
    LEARN_CHECK(buf.size() == 5);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string buf;
    buf.resize_and_overwrite(8, [](char* p, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            p[i] = '0';
        }
        return std::size_t{3};
    });
    LEARN_CHECK(buf == "000");
    LEARN_CHECK(buf.size() == 3);
#else
    std::string buf(3, '0');
    LEARN_CHECK(buf == "000");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_resize_and_overwrite_cpp23", run>;

}  // namespace
