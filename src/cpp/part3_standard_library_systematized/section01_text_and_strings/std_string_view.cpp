// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_view
// Topic id : part3/section01/std_string_view
//
// Covers: std::string_view non-owning view, substr, lifetime caution

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    const char* lit = "hello";
    std::string_view sv = lit;
    assert(sv.size() == 5);
    assert(sv[0] == 'h');
    assert(sv.substr(1, 3) == "ell");
}

void demo_intermediate() {
    std::string owner = "owned-data";
    std::string_view sv = owner;
    assert(sv == "owned-data");
    assert(sv.starts_with("owned"));
    assert(sv.ends_with("data"));
    assert(sv.find("data") == 6);
}

void demo_expert() {
    std::string_view empty;
    assert(empty.empty());
    std::string_view a = "abc";
    std::string_view b = a.substr(1);
    assert(b == "bc");
    // dangling risk: never return string_view to a temporary string
    std::string tmp = "safe";
    std::string_view keep = tmp;
    assert(keep == "safe");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_view", run>;

}  // namespace
