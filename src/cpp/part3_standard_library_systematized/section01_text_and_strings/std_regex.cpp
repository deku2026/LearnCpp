// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_regex
// Topic id : part3/section01/std_regex
//
// Covers: std::regex match/search/replace basics

#include "learn/topic_registry.hpp"

#include <cassert>
#include <regex>
#include <string>

namespace {

void demo_basics() {
    std::regex re{"[0-9]+"};
    assert(std::regex_match("123", re));
    assert(!std::regex_match("12a", re));
}

void demo_intermediate() {
    std::string text = "id=42; id=7";
    std::regex re{"[0-9]+"};
    std::smatch m;
    assert(std::regex_search(text, m, re));
    assert(m.str() == "42");
}

void demo_expert() {
    std::string text = "a1b2c";
    std::regex re{"[0-9]"};
    std::string out = std::regex_replace(text, re, "#");
    assert(out == "a#b#c");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_regex", run>;

}  // namespace
