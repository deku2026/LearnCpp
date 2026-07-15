// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_compare_and_locale_pitfalls
// Topic id : part3/section01/string_compare_and_locale_pitfalls
//
// Covers: string compare is code-unit order; locale affects iostream not operator==

#include "learn/topic_registry.hpp"

#include <cassert>
#include <locale>
#include <string>

namespace {

void demo_basics() {
    assert(std::string{"a"} < std::string{"b"});
    assert(std::string{"A"} < std::string{"a"});   // ASCII order
    assert(std::string{"10"} < std::string{"2"});  // lexicographic, not numeric
}

void demo_intermediate() {
    std::string a = "café";
    std::string b = "cafe";
    // byte-wise equality depends on encoding of source
    assert(a != b || a == b);
    assert(a.compare(b) != 0 || a == b);
}

void demo_expert() {
    // classic: operator== ignores locale; collate facets affect locale-aware compare
    std::locale loc = std::locale::classic();
    assert(std::use_facet<std::collate<char>>(loc).compare("a", "a" + 1, "b", "b" + 1) < 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_compare_and_locale_pitfalls", run>;

}  // namespace
