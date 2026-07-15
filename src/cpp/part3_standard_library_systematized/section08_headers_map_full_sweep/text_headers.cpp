// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : text_headers
// Topic id : part3/section08/text_headers
//
// Covers: headers map: string string_view charconv format regex cctype

#include "learn/topic_registry.hpp"

#include <cctype>
#include <charconv>
#include <regex>
#include <string>
#include <string_view>
#include <version>

namespace {

void demo_basics() {
    std::string s = "ab";
    std::string_view sv = s;
    LEARN_CHECK(sv.size() == 2);
}

void demo_intermediate() {
    LEARN_CHECK(std::isdigit(static_cast<unsigned char>('9')));
    char buf[8]{};
    auto r = std::to_chars(buf, buf + 8, 5);
    LEARN_CHECK(r.ec == std::errc{});
}

void demo_expert() {
    LEARN_CHECK(std::regex_match("12", std::regex{"[0-9]+"}));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/text_headers", run>;

}  // namespace
