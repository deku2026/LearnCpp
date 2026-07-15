// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : to_chars_from_chars
// Topic id : part3/section01/to_chars_from_chars
//
// Covers: std::to_chars / from_chars locale-independent conversion

#include "learn/topic_registry.hpp"

#include <charconv>
#include <cstring>
#include <string>

namespace {

void demo_basics() {
    char buf[32]{};
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), 42);
    LEARN_CHECK(ec == std::errc{});
    LEARN_CHECK(std::string(buf, ptr) == "42");
}

void demo_intermediate() {
    const char* s = "12345";
    int value = 0;
    auto [ptr, ec] = std::from_chars(s, s + 5, value);
    LEARN_CHECK(ec == std::errc{});
    LEARN_CHECK(value == 12345);
    LEARN_CHECK(ptr == s + 5);
}

void demo_expert() {
    char buf[32]{};
    auto r1 = std::to_chars(buf, buf + sizeof(buf), 255, 16);
    LEARN_CHECK(r1.ec == std::errc{});
    LEARN_CHECK(std::string(buf, r1.ptr) == "ff");
    const char* hex = "ff";
    int v = 0;
    auto r2 = std::from_chars(hex, hex + 2, v, 16);
    LEARN_CHECK(r2.ec == std::errc{});
    LEARN_CHECK(v == 255);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/to_chars_from_chars", run>;

}  // namespace
