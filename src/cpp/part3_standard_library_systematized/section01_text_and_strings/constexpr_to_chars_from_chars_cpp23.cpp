// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : constexpr_to_chars_from_chars_cpp23
// Topic id : part3/section01/constexpr_to_chars_from_chars_cpp23
//
// Covers: constexpr charconv where supported (C++23)

#include "learn/topic_registry.hpp"

#include <charconv>
#include <version>

namespace {

void demo_basics() {
    char buf[16]{};
    auto [p, ec] = std::to_chars(buf, buf + 16, 7);
    LEARN_CHECK(ec == std::errc{});
    LEARN_CHECK(p - buf == 1);
    LEARN_CHECK(buf[0] == '7');
}

void demo_intermediate() {
    const char in[] = "99";
    int v = 0;
    auto [p, ec] = std::from_chars(in, in + 2, v);
    LEARN_CHECK(ec == std::errc{});
    LEARN_CHECK(v == 99);
}

void demo_expert() {
#if defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L
    constexpr auto ok = [] {
        char b[8]{};
        auto r = std::to_chars(b, b + 8, 3);
        return r.ec == std::errc{} && b[0] == '3';
    }();
    LEARN_CHECK(ok);
#else
    char b[8]{};
    auto r = std::to_chars(b, b + 8, 3);
    LEARN_CHECK(r.ec == std::errc{} && b[0] == '3');
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/constexpr_to_chars_from_chars_cpp23", run>;

}  // namespace
