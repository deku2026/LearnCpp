// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : utf8_source_file_encoding
// Topic id : part2/stage02/section10/utf8_source_file_encoding
//
// Covers: UTF-8 source encoding, u8 string literals, char8_t (C++20)

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <version>

namespace {

void demo_basics() {
    // Ordinary string literal encoding is implementation-defined; keep ASCII for portability.
    const char* ascii = "LearnCpp";
    LEARN_CHECK(ascii[0] == 'L');
    LEARN_CHECK(std::string_view(ascii) == "LearnCpp");

    // UTF-8 string literal
    const auto* u8s = u8"ABC";
    LEARN_CHECK(u8s[0] == u8'A');
    LEARN_CHECK(u8s[1] == u8'B');
    LEARN_CHECK(u8s[2] == u8'C');
}

void demo_intermediate() {
#if defined(__cpp_char8_t)
    static_assert(std::is_same_v<decltype(u8"x"[0]), const char8_t&> || std::is_same_v<decltype(u8"x"[0]), char8_t>);
    const char8_t* p = u8"hi";
    LEARN_CHECK(p[0] == u8'h');
    LEARN_CHECK(p[1] == u8'i');

    // Code units for simple ASCII UTF-8 are identical values
    LEARN_CHECK(static_cast<unsigned char>(u8'A') == 65);
#else
    const char* p = reinterpret_cast<const char*>(u8"hi");
    LEARN_CHECK(p[0] == 'h');
#endif

    // UTF-16 / UTF-32 literals for contrast
    const char16_t* u16 = u"A";
    const char32_t* u32 = U"A";
    LEARN_CHECK(u16[0] == u'A');
    LEARN_CHECK(u32[0] == U'A');
}

void demo_expert() {
    // Prefer explicit UTF-8 APIs when handling Unicode text.
    std::string utf8_bytes = reinterpret_cast<const char*>(u8"XYZ");
    LEARN_CHECK(utf8_bytes == "XYZ");

#if defined(__cpp_lib_char8_t) && __cpp_lib_char8_t >= 201907L
    std::u8string s = u8"demo";
    LEARN_CHECK(s.size() == 4);
    LEARN_CHECK(s[0] == u8'd');
#else
    // Fallback: treat UTF-8 data as bytes in std::string
    std::string s = "demo";
    LEARN_CHECK(s.size() == 4);
#endif

    // Source file should be saved as UTF-8; keep identifiers ASCII for max toolchain portability.
    const char* id = "stage02_section10";
    LEARN_CHECK(std::string_view(id).find("section10") != std::string_view::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/utf8_source_file_encoding", run>;

}  // namespace
