// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : char_encoding_utf_prefixes
// Topic id : part3/section01/char_encoding_utf_prefixes
// Reference: C++ working draft [lex.string], [basic.fundamental].

#include "learn/example_support.hpp"

#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#else
#define LEARN_HAS_STRING_VIEW_HEADER 0
#endif

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/char_encoding_utf_prefixes";

#if LEARN_HAS_STRING_VIEW_HEADER && defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
    learn::ExampleChecks checks{topic};
    static_assert(std::same_as<decltype(u8"x"[0]), const char8_t&>);
    static_assert(std::same_as<decltype(u"x"[0]), const char16_t&>);
    static_assert(std::same_as<decltype(U"x"[0]), const char32_t&>);
    static_assert(std::same_as<decltype(L"x"[0]), const wchar_t&>);

    constexpr std::u8string_view utf8 = u8"\U0001F600";
    constexpr std::u16string_view utf16 = u"\U0001F600";
    constexpr std::u32string_view utf32 = U"\U0001F600";
    LEARN_EXPECT_EQ(checks, utf8.size(), std::size_t{4});
    LEARN_EXPECT_EQ(checks, utf16.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, utf32.size(), std::size_t{1});

    constexpr std::string_view encoded_bytes = "h\xC3\xA9llo";
    LEARN_EXPECT_EQ(checks, encoded_bytes.size(), std::size_t{6});
    // Sizes count code units. They do not count code points, grapheme clusters, or display columns.
    // wchar_t width and encoding are platform-specific; do not use it as a portable Unicode scalar type.
#if 0
    const char* wrong = u8"UTF-8";  // Since C++20, char8_t is intentionally distinct from char.
#endif
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "<string_view> and C++20 char8_t");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/char_encoding_utf_prefixes", run>;

}  // namespace
