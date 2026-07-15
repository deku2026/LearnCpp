// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_view
// Topic id : part3/section01/std_string_view
//
// Covers: std::string_view non-owning view, substr, lifetime caution

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    const char* lit = "hello";
    std::string_view sv = lit;
    LEARN_CHECK(sv.size() == 5);
    LEARN_CHECK(sv[0] == 'h');
    LEARN_CHECK(sv.substr(1, 3) == "ell");
}

void demo_intermediate() {
    std::string owner = "owned-data";
    std::string_view sv = owner;
    LEARN_CHECK(sv == "owned-data");
    LEARN_CHECK(sv.starts_with("owned"));
    LEARN_CHECK(sv.ends_with("data"));
    LEARN_CHECK(sv.find("data") == 6);
}

void demo_expert() {
    std::string_view empty;
    LEARN_CHECK(empty.empty());
    std::string_view a = "abc";
    std::string_view b = a.substr(1);
    LEARN_CHECK(b == "bc");
    // dangling risk: never return string_view to a temporary string
    std::string tmp = "safe";
    std::string_view keep = tmp;
    LEARN_CHECK(keep == "safe");
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
