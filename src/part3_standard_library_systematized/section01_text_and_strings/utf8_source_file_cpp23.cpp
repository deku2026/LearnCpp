// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : utf8_source_file_cpp23
// Topic id : part3/section01/utf8_source_file_cpp23
// Reference: C++23 working draft [lex.charset], [lex.string], [string.view].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/utf8_source_file_cpp23";

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
    learn::ExampleChecks checks{topic};

    // Universal-character names make this source independent of an editor's display encoding.
    // The u8 prefix specifies UTF-8 code units in the literal's ordinary encoding.
    static constexpr char8_t greeting[] = u8"\u4F60\u597D";  // U+4F60 U+597D
    constexpr std::u8string_view text{greeting};
    constexpr std::array expected{char8_t{0xE4}, char8_t{0xBD}, char8_t{0xA0},
                                  char8_t{0xE5}, char8_t{0xA5}, char8_t{0xBD}};

    LEARN_EXPECT_EQ(checks, text.size(), std::size_t{6});
    for (std::size_t index = 0; index < expected.size(); ++index) {
        LEARN_EXPECT_EQ(checks, text[index], expected[index]);
    }

    // size() counts UTF-8 code units, not Unicode scalar values or grapheme clusters.
    std::size_t scalar_count{};
    for (const char8_t unit : text) {
        const auto byte = static_cast<unsigned char>(unit);
        scalar_count += (byte & 0b1100'0000U) != 0b1000'0000U;
    }
    LEARN_EXPECT_EQ(checks, scalar_count, std::size_t{2});

    static_assert(std::is_same_v<decltype(u8"x"), const char8_t (&)[2]>);
    static_assert(!std::is_convertible_v<const char8_t*, const char*>);
    // char8_t prevents silently passing UTF-8 data to a narrow-character API. Conversion at an
    // I/O boundary must therefore be explicit and must respect that API's expected encoding.
#if 0
    const char* incorrectly_retyped = u8"text";  // Ill-formed since C++20: pointer types differ.
#endif
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "__cpp_char8_t >= 201811L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/utf8_source_file_cpp23", run>;

}  // namespace
