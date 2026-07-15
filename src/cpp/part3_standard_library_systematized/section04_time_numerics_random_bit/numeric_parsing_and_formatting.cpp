// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numeric_parsing_and_formatting
// Topic id : part3/section04/numeric_parsing_and_formatting
//
// Covers: stoi/stod vs from_chars/to_chars and format

#include "learn/topic_registry.hpp"

#include <charconv>
#include <string>
#include <version>

namespace {

void demo_basics() {
    LEARN_CHECK(std::stoi("42") == 42);
    LEARN_CHECK(std::stod("3.5") == 3.5);
}

void demo_intermediate() {
    char buf[16]{};
    auto [p, ec] = std::to_chars(buf, buf + 16, 100);
    LEARN_CHECK(ec == std::errc{});
    int v = 0;
    auto r = std::from_chars(buf, p, v);
    LEARN_CHECK(r.ec == std::errc{} && v == 100);
}

void demo_expert() {
    // from_chars does not skip whitespace; stoi does
    try {
        (void)std::stoi(" 7");
        LEARN_CHECK(true);
    } catch (...) {
        LEARN_CHECK(false);
    }
    const char* s = " 7";
    int v = 0;
    auto r = std::from_chars(s, s + 2, v);
    LEARN_CHECK(r.ec != std::errc{} || v == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/numeric_parsing_and_formatting", run>;

}  // namespace
