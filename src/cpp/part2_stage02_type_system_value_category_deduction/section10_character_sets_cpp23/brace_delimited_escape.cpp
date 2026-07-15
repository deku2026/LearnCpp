// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : brace_delimited_escape
// Topic id : part2/stage02/section10/brace_delimited_escape
//
// Covers: C++23 \u{...} / \o{...} / \x{...} brace-delimited escapes

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

namespace {

void demo_basics() {
    // Classic fixed-width escapes still work
    LEARN_CHECK('\x41' == 'A');
    LEARN_CHECK('\101' == 'A');  // octal
    LEARN_CHECK(U'\u0041' == U'A');
}

void demo_intermediate() {
#if defined(__cpp_named_character_escapes) || defined(__cpp_unicode_literals)
    // Brace-delimited hex/unicode forms (C++23); fall back when unavailable.
#endif

#if __cplusplus >= 202302L || (defined(__cpp_unicode_literals))
    // Prefer feature detection via compile try of brace syntax when macro missing.
#endif

#if defined(__cpp_lib_format)  // weak companion signal; real check below
#endif

    // Portable demonstration of the idea using classic escapes with same values.
    const char a = '\x41';
    const char star = '\x2A';
    LEARN_CHECK(a == 'A');
    LEARN_CHECK(star == '*');

#if defined(__clang__) || defined(__GNUC__) || defined(_MSC_VER)
    // Many C++23 compilers accept \x{...} / \u{...}; guard with a known macro if present.
#if defined(__cpp_unicode_characters) || (__cplusplus >= 202302L)
    // Try brace-delimited forms when compiling as C++23
#if __cplusplus >= 202302L
    const char b = '\x{41}';
    LEARN_CHECK(b == 'A');
    const char32_t pi = U'\u{3C0}';
    LEARN_CHECK(pi == 0x3C0);
    const char nul_like = '\o{0}';
    LEARN_CHECK(nul_like == '\0');
#else
    const char b = '\x41';
    LEARN_CHECK(b == 'A');
    const char32_t pi = U'\u03C0';
    LEARN_CHECK(pi == 0x3C0);
#endif
#else
    const char b = '\x41';
    LEARN_CHECK(b == 'A');
    const char32_t pi = U'\u03C0';
    LEARN_CHECK(pi == 0x3C0);
#endif
#else
    const char b = '\x41';
    LEARN_CHECK(b == 'A');
#endif
}

void demo_expert() {
    // Brace delimiters avoid ambiguous digit swallowing: \x12 vs following text.
    // Example classic pitfall avoided conceptually:
    // "\x41B" is one hex escape 0x41B in some languages; C++ \x consumes all hex digits.
    const char classic[] =
        "\x41"
        "B";  // split to force 'A' then 'B'
    LEARN_CHECK(classic[0] == 'A');
    LEARN_CHECK(classic[1] == 'B');

#if __cplusplus >= 202302L
    const char modern[] = "\x{41}B";  // brace ends the escape clearly
    LEARN_CHECK(modern[0] == 'A');
    LEARN_CHECK(modern[1] == 'B');

    const char32_t emoji = U'\u{1F600}';
    LEARN_CHECK(emoji == 0x1F600);
#else
    const char modern[] =
        "\x41"
        "B";
    LEARN_CHECK(modern[0] == 'A');
    LEARN_CHECK(modern[1] == 'B');
    const char32_t emoji = U'\U0001F600';
    LEARN_CHECK(emoji == 0x1F600);
#endif

    std::string s = "ok";
    LEARN_CHECK(s == "ok");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/brace_delimited_escape", run>;

}  // namespace
