// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : utf8_source_file_cpp23
// Topic id : part3/section01/utf8_source_file_cpp23
//
// Covers: UTF-8 source encoding and portable string literals

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

void demo_basics() {
    std::string ascii = "ASCII only is portable";
    assert(ascii.find("portable") != std::string::npos);
}

void demo_intermediate() {
    // Prefer u8 literals when encoding matters
    const char8_t* u8 = u8"ok";
    assert(u8[0] == u8'o');
    assert(u8[1] == u8'k');
}

void demo_expert() {
    // Named/delimited escapes (C++23) when available — ASCII fallback always works
    std::string hex = "\x41";
    assert(hex == "A");
#if defined(__cpp_named_character_escapes)
    std::string named = "\N{LATIN CAPITAL LETTER A}";
    assert(named == "A");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/utf8_source_file_cpp23", run>;

}  // namespace
