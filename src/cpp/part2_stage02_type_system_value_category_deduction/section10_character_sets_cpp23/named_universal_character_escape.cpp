// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : named_universal_character_escape
// Topic id : part2/stage02/section10/named_universal_character_escape
//
// Covers: C++23 \N{NAME} named universal character escapes; fallbacks

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

namespace {

void demo_basics() {
    // Portable numeric universal character names (long available)
    const char32_t alpha = U'\u03B1';  // Greek small letter alpha
    LEARN_CHECK(alpha == 0x03B1);

    const char32_t smile = U'\U0001F600';  // grinning face
    LEARN_CHECK(smile == 0x1F600);
}

void demo_intermediate() {
#if defined(__cpp_named_character_escapes) && __cpp_named_character_escapes >= 202207L
    // C++23 named escapes
    const char32_t pi = U'\N{GREEK SMALL LETTER PI}';
    LEARN_CHECK(pi == 0x03C0);

    const char32_t snow = U'\N{SNOWMAN}';
    LEARN_CHECK(snow == 0x2603);

    const char* latin = "\N{LATIN SMALL LETTER A}";
    LEARN_CHECK(latin[0] == 'a');
#else
    const char32_t pi = U'\u03C0';
    LEARN_CHECK(pi == 0x03C0);
    const char32_t snow = U'\u2603';
    LEARN_CHECK(snow == 0x2603);
    const char* latin = "a";
    LEARN_CHECK(latin[0] == 'a');
#endif
}

void demo_expert() {
#if defined(__cpp_named_character_escapes) && __cpp_named_character_escapes >= 202207L
    std::string note = "angle: \N{DEGREE SIGN}";
    LEARN_CHECK(note.find("angle:") != std::string::npos);
    // Degree sign in UTF-8 is multi-byte; ensure non-empty payload after prefix
    LEARN_CHECK(note.size() > 7);

    const char32_t infinity = U'\N{INFINITY}';
    LEARN_CHECK(infinity == 0x221E);
#else
    // Numeric form remains the portable baseline
    std::string note = "angle: \u00B0";
    LEARN_CHECK(note.find("angle:") != std::string::npos);
    LEARN_CHECK(note.size() > 7);

    const char32_t infinity = U'\u221E';
    LEARN_CHECK(infinity == 0x221E);
#endif

    // Named escapes improve readability for documentation-heavy code when supported.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/named_universal_character_escape", run>;

}  // namespace
